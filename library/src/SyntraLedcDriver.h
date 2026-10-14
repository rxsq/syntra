#pragma once

#include <Arduino.h>
#include "SyntraServo.h"

namespace syntra {

// Requires Arduino-ESP32 core 3.x (pin-based ledcAttach / ledcWrite API).
class LedcServoDriver : public ServoDriver {
public:
    explicit LedcServoDriver(uint8_t pin, uint32_t frequencyHz = 50,
                             uint8_t resolutionBits = 14)
        : pin_(pin), frequencyHz_(frequencyHz), resolutionBits_(resolutionBits) {}

    bool begin() override;
    void writePulseUs(uint16_t microseconds) override;

private:
    uint8_t pin_;
    uint32_t frequencyHz_;
    uint8_t resolutionBits_;
    bool attached_ = false;
};

} // namespace syntra
