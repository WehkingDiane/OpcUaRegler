#include "opcuaregler/process_simulation.hpp"

#include <algorithm>
#include <cmath>

namespace opcuaregler {

double advanceFirstOrderProcess(
    double actualValue,
    double outputValue,
    double disturbance,
    double timeConstantSeconds,
    std::chrono::duration<double> elapsed) {
    const double safeTimeConstant = std::max(0.1, timeConstantSeconds);
    const double safeElapsed = std::max(0.0, elapsed.count());
    const double target = outputValue + disturbance;
    const double alpha = 1.0 - std::exp(-safeElapsed / safeTimeConstant);
    return actualValue + ((target - actualValue) * alpha);
}

} // namespace opcuaregler
