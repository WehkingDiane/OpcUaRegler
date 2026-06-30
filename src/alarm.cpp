#include "opcuaregler/alarm.hpp"

#include <algorithm>
#include <utility>

namespace opcuaregler {

void AlarmManager::raise(int code, Severity severity, std::string text) {
    auto it = std::find_if(alarms_.begin(), alarms_.end(), [code](const Alarm& alarm) {
        return alarm.code == code;
    });

    if (it == alarms_.end()) {
        alarms_.push_back(Alarm{code, severity, std::move(text), true, false});
        return;
    }

    it->severity = severity;
    it->text = std::move(text);
    it->active = true;
}

void AlarmManager::clear(int code) {
    auto it = std::find_if(alarms_.begin(), alarms_.end(), [code](const Alarm& alarm) {
        return alarm.code == code;
    });

    if (it != alarms_.end()) {
        it->active = false;
    }
}

void AlarmManager::acknowledge(int code) {
    auto it = std::find_if(alarms_.begin(), alarms_.end(), [code](const Alarm& alarm) {
        return alarm.code == code;
    });

    if (it != alarms_.end()) {
        it->acknowledged = true;
    }
}

const std::vector<Alarm>& AlarmManager::alarms() const noexcept {
    return alarms_;
}

bool AlarmManager::hasActiveError() const noexcept {
    return std::any_of(alarms_.begin(), alarms_.end(), [](const Alarm& alarm) {
        return alarm.active && alarm.severity == Severity::Error;
    });
}

bool AlarmManager::hasActiveWarning() const noexcept {
    return std::any_of(alarms_.begin(), alarms_.end(), [](const Alarm& alarm) {
        return alarm.active && alarm.severity == Severity::Warning;
    });
}

} // namespace opcuaregler

