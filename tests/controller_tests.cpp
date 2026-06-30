#include "opcuaregler/controller.hpp"
#include "opcuaregler/euromap83_model.hpp"
#include "opcuaregler/process_simulation.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

using namespace opcuaregler;

namespace {

ControllerConfig baseConfig() {
    ControllerConfig config;
    config.setpoint = 100.0;
    config.toleranceWarning = 2.0;
    config.toleranceError = 5.0;
    config.kp = 1.0;
    config.ki = 0.0;
    config.outputMin = 0.0;
    config.outputMax = 100.0;
    config.maxOutputChangePerSecond = 1000.0;
    return config;
}

void testRunningInsideTolerance() {
    WorkingPointController controller{baseConfig()};
    const auto result = controller.update(99.0, std::chrono::milliseconds{100});
    assert(result.process.state == ControllerState::Running);
    assert(std::abs(result.process.deviation - 1.0) < 1e-9);
}

void testWarningTolerance() {
    WorkingPointController controller{baseConfig()};
    const auto result = controller.update(97.0, std::chrono::milliseconds{100});
    assert(result.process.state == ControllerState::Warning);
}

void testErrorTolerance() {
    WorkingPointController controller{baseConfig()};
    const auto result = controller.update(90.0, std::chrono::milliseconds{100});
    assert(result.process.state == ControllerState::Error);
}

void testDisabled() {
    WorkingPointController controller{baseConfig()};
    controller.setEnabled(false);
    const auto result = controller.update(90.0, std::chrono::milliseconds{100});
    assert(result.process.state == ControllerState::Disabled);
}

void testBadQuality() {
    WorkingPointController controller{baseConfig()};
    controller.setQualityGood(false);
    const auto result = controller.update(100.0, std::chrono::milliseconds{100});
    assert(result.process.state == ControllerState::Error);
}

void testSimulationNodesAreWritable() {
    Euromap83WorkingPointModel model;
    const auto nodes = model.nodes();

    const auto isWritable = [&nodes](const std::string& path) {
        for (const auto& node : nodes) {
            if (node.path == path) {
                return node.writable;
            }
        }
        return false;
    };

    assert(isWritable("Regler/Simulation/Enabled"));
    assert(isWritable("Regler/Simulation/ActualValue"));
    assert(isWritable("Regler/Simulation/Disturbance"));
    assert(isWritable("Regler/Simulation/TimeConstantSeconds"));
    assert(isWritable("Regler/Simulation/Reset"));
}

void testFirstOrderSimulationMovesTowardOutput() {
    const double next = advanceFirstOrderProcess(
        0.0,
        100.0,
        0.0,
        5.0,
        std::chrono::duration<double>{1.0});

    assert(next > 0.0);
    assert(next < 100.0);
}

void testFirstOrderSimulationAppliesDisturbance() {
    const double withoutDisturbance = advanceFirstOrderProcess(
        50.0,
        100.0,
        0.0,
        5.0,
        std::chrono::duration<double>{1.0});
    const double withDisturbance = advanceFirstOrderProcess(
        50.0,
        100.0,
        10.0,
        5.0,
        std::chrono::duration<double>{1.0});

    assert(withDisturbance > withoutDisturbance);
}

void testFirstOrderSimulationIgnoresNegativeElapsedTime() {
    const double next = advanceFirstOrderProcess(
        50.0,
        100.0,
        0.0,
        5.0,
        std::chrono::duration<double>{-1.0});

    assert(std::abs(next - 50.0) < 1e-9);
}

} // namespace

int main() {
    testRunningInsideTolerance();
    testWarningTolerance();
    testErrorTolerance();
    testDisabled();
    testBadQuality();
    testSimulationNodesAreWritable();
    testFirstOrderSimulationMovesTowardOutput();
    testFirstOrderSimulationAppliesDisturbance();
    testFirstOrderSimulationIgnoresNegativeElapsedTime();
    std::cout << "controller tests passed\n";
    return 0;
}
