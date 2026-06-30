#pragma once

#include <string>
#include <vector>

namespace opcuaregler {

enum class Severity {
    Info,
    Warning,
    Error
};

struct Alarm {
    int code{};
    Severity severity{Severity::Info};
    std::string text;
    bool active{false};
    bool acknowledged{false};
};

class AlarmManager {
public:
    void raise(int code, Severity severity, std::string text);
    void clear(int code);
    void acknowledge(int code);

    [[nodiscard]] const std::vector<Alarm>& alarms() const noexcept;
    [[nodiscard]] bool hasActiveError() const noexcept;
    [[nodiscard]] bool hasActiveWarning() const noexcept;

private:
    std::vector<Alarm> alarms_;
};

} // namespace opcuaregler

