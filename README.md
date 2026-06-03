# MorayArlo.
Arlo Soud is a Open Source ESP32-powered Bluetooth speaker with OLED display, music metadata, clock, and I2S audio output.


## Features

- Bluetooth audio streaming
- OLED display interface
- Song title display
- Artist name display
- Real-time clock
- I2S audio output
- ESP32 based
- Compact and customizable design

## Hardware

- ESP32 Dev Module
- MAX98357A I2S Amplifier
- SSD1306 OLED Display (128×64)
- Speaker (4Ω or 8Ω)

## Wiring

### SSD1306 OLED

| OLED | ESP32 |
|------|--------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### MAX98357A

| MAX98357A | ESP32 |
|-----------|--------|
| VIN | 5V |
| GND | GND |
| DIN | GPIO 27 |
| BCLK | GPIO 26 |
| LRC | GPIO 25 |

## Bluetooth Device Name

```text
Arlo Sound
```

## User Interface

### Standby Screen

When no device is connected:

- Bluetooth ready status
- Waiting for connection

### Music Screen

When a device is connected:

- Current track title
- Artist name
- Clock
- Music playback interface

## Software Requirements

Required libraries:

- Adafruit GFX
- Adafruit SSD1306
- AudioTools
- ESP32-A2DP

## Installation

1. Install Arduino IDE.
2. Install ESP32 Board Support Package.
3. Install the required libraries.
4. Upload the firmware to the ESP32.
5. Pair your device with "Arlo Sound".

## Project Status

Active development.

## Contributing

Contributions, suggestions, and improvements are welcome.

## Author

Rayane Mahmoud

## License

Open Source Project

Feel free to use, modify, learn from, and share this project.
Credit is appreciated but not required.
