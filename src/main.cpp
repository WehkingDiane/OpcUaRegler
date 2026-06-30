#include "opcuaregler/config.hpp"
#include "opcuaregler/controller.hpp"
#include "opcuaregler/euromap83_model.hpp"
#include "opcuaregler/opcua_backend.hpp"

#include <chrono>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

volatile std::sig_atomic_t g_stopRequested = 0;

void handleSignal(int) {
    g_stopRequested = 1;
}

std::filesystem::path configPathFromArgs(int argc, char** argv) {
    if (argc >= 3 && std::string{argv[1]} == "--config") {
        return argv[2];
    }
    return "config/regler.example.toml";
}

} // namespace

int main(int argc, char** argv) {
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    try {
        const auto config = opcuaregler::loadConfig(configPathFromArgs(argc, argv));
        opcuaregler::Euromap83WorkingPointModel model;
        opcuaregler::WorkingPointController controller{config.controller};

        std::unique_ptr<opcuaregler::OpcUaBackend> backend;
        if (config.simulation) {
            backend = std::make_unique<opcuaregler::MemoryOpcUaBackend>();
        } else {
#if defined(OPCUAREGLER_WITH_OPEN62541)
            backend = std::make_unique<opcuaregler::Open62541OpcUaBackend>(
                config.opcua.port,
                config.opcua.applicationUri);
#else
            throw std::runtime_error("Config disables simulation, but the binary was built without open62541 support");
#endif
        }

        backend->start(model);
        backend->publish(controller.process(), {});

        while (!g_stopRequested) {
            controller.setSetpoint(backend->readSetpoint(controller.process().setpoint));
            controller.setQualityGood(backend->readQualityGood());
            controller.setEnabled(backend->readEnabled());
            controller.setManual(
                backend->readManualMode(),
                backend->readManualOutput(controller.process().outputValue));

            if (backend->readAcknowledgeRequested()) {
                controller.acknowledgeAlarms();
            }

            const auto actualValue = backend->readActualValue();
            const auto result = controller.update(actualValue, config.controller.cycleTime);
            backend->publish(result.process, result.alarms);

            std::this_thread::sleep_for(config.controller.cycleTime);
        }

        backend->stop();
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
}
