#pragma once

#include "opcuaregler/alarm.hpp"
#include "opcuaregler/config.hpp"

#include <chrono>
#include <string>

namespace opcuaregler {

enum class ControllerState {
    Init,
    Ready,
    Running,
    Warning,
    Error,
    Manual,
    Disabled
};

struct ProcessImage {
    double setpoint{};
    double actualValue{};
    double deviation{};
    double outputValue{};
    bool enabled{true};
    bool manual{false};
    double manualOutput{};
    bool qualityGood{true};
    ControllerState state{ControllerState::Init};
};

struct ControllerResult {
    ProcessImage process;
    std::vector<Alarm> alarms;
};

class WorkingPointController {
public:
    explicit WorkingPointController(ControllerConfig config);

    ControllerResult update(double actualValue, std::chrono::milliseconds elapsed);
    void setEnabled(bool enabled);
    void setManual(bool manual, double output);
    void setSetpoint(double setpoint);
    void setQualityGood(bool qualityGood);
    void acknowledgeAlarms();
    [[nodiscard]] const ProcessImage& process() const noexcept;

private:
    ControllerConfig config_;
    ProcessImage process_;
    AlarmManager alarms_;
    double integral_{0.0};

    [[nodiscard]] double clampOutput(double value) const noexcept;
    [[nodiscard]] double limitOutputRate(double requested, std::chrono::milliseconds elapsed) const noexcept;
    void evaluateLimits();
};

std::string toString(ControllerState state);

} // namespace opcuaregler

