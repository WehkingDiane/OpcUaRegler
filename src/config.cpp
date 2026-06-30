#include "opcuaregler/config.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace opcuaregler {
namespace {

std::string trim(std::string_view value) {
    auto begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos) {
        return {};
    }
    auto end = value.find_last_not_of(" \t\r\n");
    return std::string{value.substr(begin, end - begin + 1)};
}

std::string stripQuotes(std::string value) {
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

double parseDouble(const std::string& value) {
    double parsed{};
    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), parsed);
    if (ec != std::errc{} || ptr != value.data() + value.size()) {
        throw std::runtime_error("Invalid double value in config: " + value);
    }
    return parsed;
}

std::uint16_t parsePort(const std::string& value) {
    unsigned parsed{};
    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), parsed);
    if (ec != std::errc{} || ptr != value.data() + value.size() || parsed > 65535U) {
        throw std::runtime_error("Invalid port value in config: " + value);
    }
    return static_cast<std::uint16_t>(parsed);
}

bool parseBool(std::string value) {
    std::ranges::transform(value, value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    if (value == "true") {
        return true;
    }
    if (value == "false") {
        return false;
    }
    throw std::runtime_error("Invalid bool value in config: " + value);
}

} // namespace

AppConfig loadConfig(const std::filesystem::path& path) {
    AppConfig config;

    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Cannot open config file: " + path.string());
    }

    std::string section;
    std::string line;
    while (std::getline(input, line)) {
        auto comment = line.find('#');
        if (comment != std::string::npos) {
            line.erase(comment);
        }
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        if (line.front() == '[' && line.back() == ']') {
            section = trim(std::string_view{line}.substr(1, line.size() - 2));
            continue;
        }

        auto equals = line.find('=');
        if (equals == std::string::npos) {
            throw std::runtime_error("Invalid config line: " + line);
        }

        auto key = trim(std::string_view{line}.substr(0, equals));
        auto value = trim(std::string_view{line}.substr(equals + 1));
        value = stripQuotes(value);

        const auto fullKey = section.empty() ? key : section + "." + key;
        if (fullKey == "service.simulation") {
            config.simulation = parseBool(value);
        } else if (fullKey == "opcua.bind_address") {
            config.opcua.bindAddress = value;
        } else if (fullKey == "opcua.port") {
            config.opcua.port = parsePort(value);
        } else if (fullKey == "opcua.application_uri") {
            config.opcua.applicationUri = value;
        } else if (fullKey == "controller.setpoint") {
            config.controller.setpoint = parseDouble(value);
        } else if (fullKey == "controller.tolerance_warning") {
            config.controller.toleranceWarning = parseDouble(value);
        } else if (fullKey == "controller.tolerance_error") {
            config.controller.toleranceError = parseDouble(value);
        } else if (fullKey == "controller.kp") {
            config.controller.kp = parseDouble(value);
        } else if (fullKey == "controller.ki") {
            config.controller.ki = parseDouble(value);
        } else if (fullKey == "controller.output_min") {
            config.controller.outputMin = parseDouble(value);
        } else if (fullKey == "controller.output_max") {
            config.controller.outputMax = parseDouble(value);
        } else if (fullKey == "controller.max_output_change_per_second") {
            config.controller.maxOutputChangePerSecond = parseDouble(value);
        } else if (fullKey == "controller.cycle_time_ms") {
            config.controller.cycleTime = std::chrono::milliseconds{static_cast<int>(parseDouble(value))};
        }
    }

    if (config.controller.toleranceWarning < 0.0 || config.controller.toleranceError < 0.0) {
        throw std::runtime_error("Tolerances must be non-negative");
    }
    if (config.controller.toleranceWarning > config.controller.toleranceError) {
        throw std::runtime_error("Warning tolerance must not be greater than error tolerance");
    }
    if (config.controller.outputMin > config.controller.outputMax) {
        throw std::runtime_error("Output min must not be greater than output max");
    }
    if (config.controller.cycleTime.count() <= 0) {
        throw std::runtime_error("Cycle time must be positive");
    }

    return config;
}

} // namespace opcuaregler
