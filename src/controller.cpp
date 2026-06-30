#include "opcuaregler/controller.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace opcuaregler {
namespace {

constexpr int kBadQualityAlarm = 1001;
constexpr int kToleranceWarningAlarm = 2001;
constexpr int kToleranceErrorAlarm = 2002;
constexpr int kOutputLimitedAlarm = 3001;

} // namespace

WorkingPointController::WorkingPointController(ControllerConfig config)
    : config_(std::move(config)) {
    process_.setpoint = config_.setpoint;
    process_.outputValue = config_.outputMin;
    process_.state = ControllerState::Ready;
}

ControllerResult WorkingPointController::update(double actualValue, std::chrono::milliseconds elapsed) {
    process_.actualValue = actualValue;
    process_.deviation = process_.setpoint - process_.actualValue;

    if (!process_.enabled) {
        process_.state = ControllerState::Disabled;
        return ControllerResult{process_, alarms_.alarms()};
    }

    if (!process_.qualityGood) {
        alarms_.raise(kBadQualityAlarm, Severity::Error, "Actual value quality is bad or stale");
        process_.state = ControllerState::Error;
        return ControllerResult{process_, alarms_.alarms()};
    }

    alarms_.clear(kBadQualityAlarm);

    if (process_.manual) {
        process_.outputValue = clampOutput(process_.manualOutput);
        process_.state = ControllerState::Manual;
        evaluateLimits();
        return ControllerResult{process_, alarms_.alarms()};
    }

    const double dtSeconds = static_cast<double>(elapsed.count()) / 1000.0;
    integral_ += process_.deviation * dtSeconds;

    const double requested = (config_.kp * process_.deviation) + (config_.ki * integral_);
    const double clamped = clampOutput(requested);
    process_.outputValue = limitOutputRate(clamped, elapsed);

    if (std::abs(clamped - requested) > 1e-9) {
        alarms_.raise(kOutputLimitedAlarm, Severity::Warning, "Controller output is limited");
        if (config_.ki != 0.0) {
            integral_ -= process_.deviation * dtSeconds;
        }
    } else {
        alarms_.clear(kOutputLimitedAlarm);
    }

    evaluateLimits();
    return ControllerResult{process_, alarms_.alarms()};
}

void WorkingPointController::setEnabled(bool enabled) {
    process_.enabled = enabled;
    if (!enabled) {
        integral_ = 0.0;
    }
}

void WorkingPointController::setManual(bool manual, double output) {
    process_.manual = manual;
    process_.manualOutput = output;
}

void WorkingPointController::setSetpoint(double setpoint) {
    process_.setpoint = setpoint;
}

void WorkingPointController::setQualityGood(bool qualityGood) {
    process_.qualityGood = qualityGood;
}

void WorkingPointController::acknowledgeAlarms() {
    for (const auto& alarm : alarms_.alarms()) {
        alarms_.acknowledge(alarm.code);
    }
}

const ProcessImage& WorkingPointController::process() const noexcept {
    return process_;
}

double WorkingPointController::clampOutput(double value) const noexcept {
    return std::clamp(value, config_.outputMin, config_.outputMax);
}

double WorkingPointController::limitOutputRate(double requested, std::chrono::milliseconds elapsed) const noexcept {
    const double maxDelta = config_.maxOutputChangePerSecond * static_cast<double>(elapsed.count()) / 1000.0;
    return std::clamp(requested, process_.outputValue - maxDelta, process_.outputValue + maxDelta);
}

void WorkingPointController::evaluateLimits() {
    const double absDeviation = std::abs(process_.deviation);

    if (absDeviation > config_.toleranceError) {
        alarms_.raise(kToleranceErrorAlarm, Severity::Error, "Working point outside error tolerance");
        alarms_.clear(kToleranceWarningAlarm);
    } else if (absDeviation > config_.toleranceWarning) {
        alarms_.clear(kToleranceErrorAlarm);
        alarms_.raise(kToleranceWarningAlarm, Severity::Warning, "Working point outside warning tolerance");
    } else {
        alarms_.clear(kToleranceErrorAlarm);
        alarms_.clear(kToleranceWarningAlarm);
    }

    if (alarms_.hasActiveError()) {
        process_.state = ControllerState::Error;
    } else if (alarms_.hasActiveWarning()) {
        process_.state = ControllerState::Warning;
    } else {
        process_.state = ControllerState::Running;
    }
}

std::string toString(ControllerState state) {
    switch (state) {
        case ControllerState::Init:
            return "INIT";
        case ControllerState::Ready:
            return "READY";
        case ControllerState::Running:
            return "RUNNING";
        case ControllerState::Warning:
            return "WARNING";
        case ControllerState::Error:
            return "ERROR";
        case ControllerState::Manual:
            return "MANUAL";
        case ControllerState::Disabled:
            return "DISABLED";
    }

    return "UNKNOWN";
}

} // namespace opcuaregler

