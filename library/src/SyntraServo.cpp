#include "SyntraServo.h"

namespace syntra {

Servo::Servo(ServoDriver& driver, ServoConfig config)
    : driver_(driver), config_(config) {
    if (config_.minAngle > config_.maxAngle) {
        const int tmp = config_.minAngle;
        config_.minAngle = config_.maxAngle;
        config_.maxAngle = tmp;
    }
    currentAngle_ = startAngle_ = targetAngle_ = clampAngle(config_.homeAngle);
}

bool Servo::begin() {
    if (config_.minAngle == config_.maxAngle ||
        config_.minPulseUs >= config_.maxPulseUs || !driver_.begin()) {
        return false;
    }
    ready_ = true;
    writeAngle(clampAngle(config_.homeAngle));
    return true;
}

int Servo::clampAngle(int angle) const {
    return constrain(angle, config_.minAngle, config_.maxAngle);
}

void Servo::writeAngle(int angle) {
    angle = clampAngle(angle);
    const int32_t pulse = map(angle, config_.minAngle, config_.maxAngle,
                              config_.minPulseUs, config_.maxPulseUs);
    driver_.writePulseUs(static_cast<uint16_t>(pulse));
    currentAngle_ = angle;
}

bool Servo::moveTo(int angle, uint32_t durationMs) {
    if (!ready_) return false;
    update(); // begin the next move from the latest interpolated position
    startAngle_ = currentAngle_;
    targetAngle_ = clampAngle(angle);
    startMs_ = millis();
    durationMs_ = durationMs;
    moving_ = durationMs != 0 && startAngle_ != targetAngle_;
    if (!moving_) writeAngle(targetAngle_);
    return true;
}

void Servo::update() {
    if (!ready_ || !moving_) return;
    const uint32_t elapsed = millis() - startMs_;
    if (elapsed >= durationMs_) {
        writeAngle(targetAngle_);
        moving_ = false;
        return;
    }
    // 64-bit arithmetic prevents overflow with large durations.
    const int64_t delta = static_cast<int64_t>(targetAngle_) - startAngle_;
    const int angle = startAngle_ +
        static_cast<int>((delta * elapsed) / durationMs_);
    if (angle != currentAngle_) writeAngle(angle);
}

void Servo::stop() {
    if (!ready_) return;
    update();
    moving_ = false;
    targetAngle_ = currentAngle_;
}

bool Servo::center(uint32_t durationMs) {
    return moveTo(config_.homeAngle, durationMs);
}

} // namespace syntra
