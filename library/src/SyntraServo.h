#pragma once

#include <Arduino.h>

namespace syntra {

// Drivers receive microsecond pulse commands; motion logic does not know
// whether the signals come from ESP32 LEDC or an external PWM controller.
class ServoDriver {
public:
    virtual ~ServoDriver() = default;
    virtual bool begin() = 0;
    virtual void writePulseUs(uint16_t microseconds) = 0;
};

struct ServoConfig {
    int minAngle = 0;
    int maxAngle = 180;
    int homeAngle = 90;
    uint16_t minPulseUs = 500;
    uint16_t maxPulseUs = 2500;
};

class Servo {
public:
    Servo(ServoDriver& driver, ServoConfig config = {});

    bool begin();
    // durationMs=0 commands the endpoint immediately (servo still has physical travel time).
    bool moveTo(int angle, uint32_t durationMs = 0);
    void update();
    void stop();
    bool center(uint32_t durationMs = 0);

    int angle() const { return currentAngle_; }
    int targetAngle() const { return targetAngle_; }
    bool moving() const { return moving_; }
    bool ready() const { return ready_; }

private:
    int clampAngle(int angle) const;
    void writeAngle(int angle);

    ServoDriver& driver_;
    ServoConfig config_;
    bool ready_ = false;
    bool moving_ = false;
    int currentAngle_ = 90;
    int startAngle_ = 90;
    int targetAngle_ = 90;
    uint32_t startMs_ = 0;
    uint32_t durationMs_ = 0;
};

} // namespace syntra
