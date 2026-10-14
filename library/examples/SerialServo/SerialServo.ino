#include <Arduino.h>
#include <Syntra.h>

// DOIT ESP32 DevKit V1: GPIO32 supports PWM. GPIO34-39 are input-only.
constexpr uint8_t SERVO_PIN = 32;

// Unmounted MG90S bench test. Keep the horn clear and do not force travel.
syntra::LedcServoDriver output(SERVO_PIN);
syntra::ServoConfig config = {30, 150, 90, 500, 2500};
syntra::Servo servo(output, config);

String line;

void handleCommand(String command) {
    command.trim();
    int angle = 0;
    unsigned long duration = 0;

    if (command == "help") {
        Serial.println("Commands: set <angle> | move <angle> <ms> | center | stop | status | help");
    } else if (command == "center") {
        servo.center(500);
        Serial.println("Centering.");
    } else if (command == "stop") {
        servo.stop();
        Serial.println("Movement stopped.");
    } else if (command == "status") {
        Serial.printf("Current=%d Target=%d Moving=%s\n", servo.angle(),
                      servo.targetAngle(), servo.moving() ? "yes" : "no");
    } else if (sscanf(command.c_str(), "move %d %lu", &angle, &duration) == 2) {
        if (duration > 60000UL) {
            Serial.println("Duration must be <= 60000 ms.");
            return;
        }
        servo.moveTo(angle, static_cast<uint32_t>(duration));
        Serial.printf("Moving to %d degrees over %lu ms (clamped to configured limits).\n",
                      angle, duration);
    } else if (sscanf(command.c_str(), "set %d", &angle) == 1) {
        servo.moveTo(angle);
        Serial.printf("Set %d degrees (clamped to configured limits).\n", angle);
    } else {
        Serial.println("Unknown command. Type help.");
    }
}

void setup() {
    Serial.begin(115200);
    if (!servo.begin()) {
        Serial.println("Servo initialization failed: check GPIO and ESP32 core version.");
        return;
    }
    Serial.println("Syntra serial servo bench test ready. Type help.");
}

void loop() {
    servo.update(); // Non-blocking: call frequently
    while (Serial.available()) {
        char c = static_cast<char>(Serial.read());
        if (c == '\n' || c == '\r') {
            if (line.length()) {
                handleCommand(line);
                line = "";
            }
        } else if (line.length() < 80) {
            line += c;
        } else {
            line = ""; // Discard oversized command
            Serial.println("Command too long.");
        }
    }
}
