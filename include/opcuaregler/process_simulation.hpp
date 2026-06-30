#pragma once

#include <chrono>

namespace opcuaregler {

[[nodiscard]] double advanceFirstOrderProcess(
    double actualValue,
    double outputValue,
    double disturbance,
    double timeConstantSeconds,
    std::chrono::duration<double> elapsed);

} // namespace opcuaregler
