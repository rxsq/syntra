# Syntra

**Syntra** is a reusable embedded motion-control library for robotics and animatronics.

> **Status: v0.1.0 foundation — not yet compiled or validated on hardware.** Initially targets ESP32 with Arduino-ESP32 core 3.x.

## Included

- Native ESP32 LEDC PWM driver; no `ESP32Servo` dependency
- Non-blocking servo position interpolation using `millis()`
- Configurable angle limits, home angle, and pulse-width calibration
- Driver abstraction allowing future PCA9685 support
- Arduino IDE serial bench-test example for one unmounted MG90S

## Project layout

- `library/src/SyntraServo.*`: servo movement and limit management
- `library/src/SyntraLedcDriver.*`: ESP32 LEDC PWM output
- `library/examples/SerialServo/SerialServo.ino`: interactive bench test
- `library/library.properties`: Arduino library metadata

The self-contained Arduino library lives entirely under `library/`, allowing other repository directories to be added later.

## Live Arduino IDE development on Windows (recommended)

Clone this repository somewhere outside the Arduino library directory. With Arduino IDE closed, open PowerShell and create a directory junction from the Arduino sketchbook's `libraries/Syntra` folder to the clone's `library` folder.

Example — **replace both paths** with your actual directories:

```powershell
$source = "C:\dev\syntra\library"
$target = "$HOME\Documents\Arduino\libraries\Syntra"

# Target must not already exist. Check your Sketchbook location in Arduino IDE Preferences.
if (-not (Test-Path -LiteralPath $source -PathType Container)) { throw "Syntra library source not found: $source" }
if (Test-Path -LiteralPath $target) { throw "Target already exists: $target" }
New-Item -ItemType Directory -Force -Path (Split-Path $target) | Out-Null
New-Item -ItemType Junction -Path $target -Target $source
```

The junction makes Arduino IDE discover the Git-tracked library directly. Update the Git checkout with `git pull`; no reinstallation is needed. Restart Arduino IDE after creating the junction. The sketchbook folder may be somewhere other than Documents, including under OneDrive; check **File → Preferences → Sketchbook location**.

To **unlink** later, remove the junction itself (not the source checkout). Never delete the files in the linked source folder.

**Do not install the whole repository ZIP as an Arduino library** after this layout change: the Arduino library root is `library/`, not the repository root.

## Quick start (Arduino IDE)

1. Install **esp32 by Espressif Systems**, version **3.x**.
2. Clone this repository and create a junction from your Arduino sketchbook's `libraries/Syntra` directory to the repository's `library/` directory (see below).
3. Open **File → Examples → Syntra → SerialServo**.
4. Select your DOIT ESP32 DevKit V1 board, then upload.
5. Open Serial Monitor at **115200 baud** with newline enabled.

Example commands:

```text
help
set 90
move 120 500
center
stop
status
```

The test uses GPIO32, 50 Hz PWM, 14-bit resolution, and a conservative 30–150° position window. The 500–2500 µs pulse-width mapping matches the MG90S packaging you reported; verify actual safe travel before installing linkages.

`move 120 500` interpolates commanded positions over 500 ms; it cannot increase the servo's physical maximum speed.

## Bench wiring

| MG90S wire | Connection |
|---|---|
| Orange (signal) | ESP32 GPIO32 |
| Red (+) | External regulated ~5V buck converter output |
| Brown (ground) | External 5V supply ground |
| ESP32 GND | Same supply ground |
| ESP32 USB | Computer for power and programming |

**Never supply 24V directly to ESP32 or servos.** Never run aggregate servo power through solderless breadboard rails. Verify regulator output first and keep the servo power supply suitably protected.

## Roadmap

1. Validate free-standing MG90S control (current).
2. Implement PCA9685 over I²C once hardware arrives.
3. Add E32 eye/eyelid mapping, blinking, and idle animations after assembly.
4. Expand to other motion systems without changing high-level servo API.

This is prototype firmware, not a safety-certified control system.
