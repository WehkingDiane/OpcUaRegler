#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <string>

namespace opcuaregler {

struct OpcUaEndpointConfig {
    std::string bindAddress{"0.0.0.0"};
    std::uint16_t port{4840};
    std::string applicationUri{"urn:opcuaregler:working-point-controller"};
};

struct ControllerConfig {
    double setpoint{100.0};
    double toleranceWarning{2.0};
    double toleranceError{5.0};
    double kp{1.0};
    double ki{0.0};
    double outputMin{0.0};
    double outputMax{100.0};
    double maxOutputChangePerSecond{10.0};
    std::chrono::milliseconds cycleTime{std::chrono::milliseconds{500}};
};

struct AppConfig {
    OpcUaEndpointConfig opcua;
    ControllerConfig controller;
    bool simulation{true};
};

AppConfig loadConfig(const std::filesystem::path& path);

} // namespace opcuaregler

