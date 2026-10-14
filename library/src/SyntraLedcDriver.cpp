#include "SyntraLedcDriver.h"

namespace syntra {

bool LedcServoDriver::begin() {
    if (frequencyHz_ == 0 || resolutionBits_ == 0 || resolutionBits_ > 16)
        return false;
    attached_ = ledcAttach(pin_, frequencyHz_, resolutionBits_);
    return attached_;
}

void LedcServoDriver::writePulseUs(uint16_t microseconds) {
    if (!attached_) return;
    const uint64_t periodUs = 1000000ULL / frequencyHz_;
    const uint32_t maxDuty = (1UL << resolutionBits_) - 1UL;
    const uint32_t duty = static_cast<uint32_t>(
        (static_cast<uint64_t>(microseconds) * maxDuty) / periodUs);
    ledcWrite(pin_, duty > maxDuty ? maxDuty : duty);
}

} // namespace syntra
