#include "opcuaregler/controller.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>

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

} // namespace

int main() {
    testRunningInsideTolerance();
    testWarningTolerance();
    testErrorTolerance();
    testDisabled();
    testBadQuality();
    std::cout << "controller tests passed\n";
    return 0;
}

