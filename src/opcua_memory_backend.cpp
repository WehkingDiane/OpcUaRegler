#include "opcuaregler/opcua_backend.hpp"

#include <cmath>
#include <iostream>

namespace opcuaregler {

void MemoryOpcUaBackend::start(const Euromap83WorkingPointModel& model) {
    running_ = true;
    std::cout << "Memory OPC UA backend started with namespace " << model.namespaceUri() << '\n';
    std::cout << "Model exposes " << model.nodes().size() << " nodes\n";
}

void MemoryOpcUaBackend::stop() {
    running_ = false;
    std::cout << "Memory OPC UA backend stopped\n";
}

void MemoryOpcUaBackend::publish(const ProcessImage& process, const std::vector<Alarm>& alarms) {
    if (!running_) {
        return;
    }

    std::cout << "state=" << toString(process.state)
              << " sp=" << process.setpoint
              << " pv=" << process.actualValue
              << " dev=" << process.deviation
              << " mv=" << process.outputValue
              << " alarms=" << alarms.size()
              << '\n';
}

double MemoryOpcUaBackend::readActualValue() {
    ++cycle_;
    simulatedActual_ += std::sin(static_cast<double>(cycle_) / 8.0) * 0.25;
    return simulatedActual_;
}

double MemoryOpcUaBackend::readSetpoint(double currentSetpoint) {
    return currentSetpoint;
}

bool MemoryOpcUaBackend::readQualityGood() {
    return true;
}

bool MemoryOpcUaBackend::readEnabled() {
    return enabled_;
}

bool MemoryOpcUaBackend::readManualMode() {
    return false;
}

double MemoryOpcUaBackend::readManualOutput(double currentOutput) {
    return currentOutput;
}

bool MemoryOpcUaBackend::readAcknowledgeRequested() {
    return false;
}

} // namespace opcuaregler
