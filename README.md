# LF_Alpha

**Embedded Firmware LF_Alpha Library for ESP32-S3 with comprehensive peripherals integration**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32-S3](https://img.shields.io/badge/Platform-ESP32--S3-blue.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)

---

## 📑 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Hardware Configuration](#hardware-configuration)
- [Third-Party Libraries](#third-party-libraries)
- [License](#license)
- [Contact Support](#contact-support)

---

## Overview

LF_Alpha is a comprehensive embedded firmware library designed specifically for the **ESP32-S3 Mini-1** microcontroller. It provides unified interfaces for multiple sensors, displays, and peripherals commonly used in IoT and embedded projects.

**Key Characteristics:**
- **Target Platform:** ESP32-S3 Mini-1 (Optimized)
- **Architecture:** C and Object-oriented C++ with modular class design
- **I2C Configuration:** SDA=GPIO38, SCL=GPIO37 (Pre-configured)
- **Interrupt Support:** Built-in interrupt handling for sensors
- **Development Environment:** Arduino IDE (v1.8.x+)

---

## Features

### Sensor Integration (8 Major Sensors)

| Sensor | Class Name | Function | I2C Address |
|--------|-----------|----------|------------|
| MPU6050 | LF_Alpha_MPU6050 | 6-axis accelerometer/gyroscope | 0x69 |
| MAX30102 | LF_Alpha_MAX30102 | Heart rate & SPO2 pulse oximeter | 0x57 |
| AHT20 | LF_Alpha_AHT20 | Temperature & humidity sensor | 0x38 |
| APDS9960 | LF_Alpha_APDS9960 | RGB color, proximity, gesture detection | 0x39 |
| DS3231 | LF_Alpha_RTC | Precision real-time clock | 0x68 |
| HC-SR04 | LF_Alpha_HCSR04 | Ultrasonic distance measurement | GPIO-based |
| SSD1306 | LF_Alpha_OLED | 128x64 OLED display | 0x3C |
| WS2812B | LF_Alpha_rgbLEDs | Addressable NeoPixel RGB LEDs | GPIO-based |

### Control & Output

| Component | Class Name | Control Method |
|-----------|-----------|----------------|
| Buzzer | LF_Alpha_Buzzer | Tone generation (GPIO18) |
| User LEDs | LF_Alpha_userLEDs | GPIO control (GPIO21, GPIO26) |
| Servo Motor | LF_Alpha_Servo | PWM (GPIO47) |
| Relays | LF_Alpha_Relays | GPIO switching (GPIO36, GPIO35) |

### Input Processing

| Component | Class Name | Interface |
|-----------|-----------|-----------|
| Buttons | LF_Alpha_Buttons | Debounced GPIO input |
| Potentiometer | LF_Alpha_resPot | ADC with calibration |

### Communication

| Protocol | Class Name | Features |
|----------|-----------|----------|
| WiFi | LF_Alpha_WiFi | Station/AP modes, connection management |
| HTTP | LF_Alpha_HTTPClient | GET/POST/PUT/DELETE requests |
| I2C | LF_Alpha_I2CScanner | Device detection & diagnosis |

---

## Installation

### Method 1: Direct Arduino IDE Installation

```
1. Download this repository as ZIP
2. In Arduino IDE: Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart Arduino IDE
```

### Method 2: Arduino Library Manager

```
1. In Arduino IDE: Sketch > Include Library > Manage Libraries
2. Search "LFAlpha"
3. Install the latest version
```

---

## Quick Start

### Test All Peripherals

Follow the steps below to run the validation routine:

#### Prerequisites

*   The Arduino IDE and ESP32 Board should be installed.
*   Your LF_Alpha Kit is connected to your computer via USB.

#### Validation Steps

1.  **Open the Example File:**
    Navigate to `File` -> `Examples` -> `LF_Alpha` -> `LF_Alpha_Validate`.

2.  **Update WiFi Credentials:**
    Switch to the `devLib.h` tab within the Arduino IDE. Locate the fields for `SSID` and `PASSWORD` and update them with your local WiFi network credentials.

3.  **Open the Serial Monitor:**
    Open the Arduino IDE's Serial Monitor tool. Ensure the baud rate is set correctly (115200 for this kit).

4.  **Enter Programming Mode:**
    Set the slide switch on the kit to **PROG** (Program/boot) mode, then press the **RESET** button.

5.  **Verify Download Status:**
    Observe the Serial Monitor for the message *"waiting for download"*. Once this message appears, close the Serial Monitor (closing it is crucial before uploading).

6.  **Upload the Code:**
    Click the **Upload** button in the Arduino IDE. Wait for the compilation and upload process to complete successfully.

7.  **Enter Execution Mode & Run:**
    After the upload is finished, open the Serial Monitor again. Switch the kit into **EXE** (Execution) mode and press **RESET**.

8.  **Wait for Calibration:**
    The MPU6050 Sensor will begin a calibration process. This typically takes around 4–5 minutes to complete.

9.  **Follow On-Screen Instructions:**
    Follow the instructions as they are displayed on the Serial Monitor and the integrated OLED screen.

10. **Review the Results:**
    Once all tests are completed, the final validation result will be displayed on the Serial Monitor/OLED.

### Example 1: Buzzer Control

```cpp
#include <LF_Alpha.h>

LF_Alpha_Buzzer buzzer;

void setup() {
    Serial.begin(115200);
    buzzer.begin();
}

void loop() {
    buzzer.beep(500);      // Beep for 500ms
    delay(1000);
    
    buzzer.write(HIGH);    // Continuous sound
    delay(500);
    buzzer.write(LOW);     // Stop sound
    delay(1000);
}
```

**Methods Available:**

| Method | Parameters | Description |
|--------|-----------|-------------|
| begin() | None | Initialize buzzer on default GPIO18 |
| begin(uint8_t pin) | GPIO pin number | Initialize on custom GPIO |
| beep(uint16_t duration) | Duration in ms | Single beep tone |
| write(bool state) | HIGH/LOW | Continuous tone control |
| end() | None | Cleanup and disable buzzer |

---

## API Reference

### Class: LF_Alpha_Buzzer

#### Methods

```cpp
LF_Alpha_Buzzer buzzer;

// Initialization
bool begin();                           // Uses GPIO18 (default)
bool begin(uint8_t customGPIO);         // Custom GPIO pin

// Control
void write(bool state);                 // HIGH = on, LOW = off
void beep(uint16_t duration = 1000);    // Duration in milliseconds

// Cleanup
bool end();                             // Disable and release resources
```

#### Usage Example
```cpp
buzzer.begin();                  // Initialize
buzzer.beep(300);               // 300ms beep
delay(500);
buzzer.beep(300);
buzzer.end();                   // Cleanup

```

---

### Class: LF_Alpha_userLEDs

#### Methods

```cpp
LF_Alpha_userLEDs leds;

// Initialization
bool begin();                           // Initialize both LEDs
bool begin(uint8_t pin);                // Custom single LED
bool begin(uint8_t led1, uint8_t led2); // Custom dual LEDs

// Control
void writeLED1(bool state);             // LED1 control (GPIO21, Active LOW)
void writeLED2(bool state);             // LED2 control (GPIO26, Active HIGH)
void write(uint8_t led, bool state);    // Generic LED control
void blink(uint8_t led, uint16_t duration); // Blink pattern

// Cleanup
bool end();
```

#### Hardware Note
- LED1 (GPIO21): Active LOW (write LOW to turn ON)
- LED2 (GPIO26): Active HIGH (write HIGH to turn ON)

---

### Class: LF_Alpha_MPU6050

#### Methods

```cpp
LF_Alpha_MPU6050 mpu;

// Initialization & Calibration
bool begin();
void calibrateSensor();
void deepCalibrateSensor();

// Motion Reading
void getMotion6(int16_t *ax, int16_t *ay, int16_t *az,
                int16_t *gx, int16_t *gy, int16_t *gz);

// Individual Axis Reading
int16_t readAccX();
int16_t readAccY();
int16_t readAccZ();
int16_t readGyroX();
int16_t readGyroY();
int16_t readGyroZ();

// Status & Configuration
bool getMotionState();
int16_t getCalibratedThreshold();
void setIdleThresholdGyro(int16_t threshold = 250);

// Cleanup
bool end();
```

#### Parameters Explanation
| Parameter | Range | Unit | Description |
|-----------|-------|------|-------------|
| ax, ay, az | -32768 to +32767 | Raw ADC | Acceleration values (3 axes) |
| gx, gy, gz | -32768 to +32767 | Raw ADC | Gyroscope values (3 axes) |
| threshold | 0-32767 | Raw ADC | Motion detection threshold |

---

### Class: LF_Alpha_resPot

#### Methods

```cpp
LF_Alpha_resPot pot;

// Initialization
bool begin();
bool begin(uint8_t adcPin);

// Reading
int16_t readAnalog();              // Raw ADC value (0-4095)
int8_t readVoltage();              // Voltage percentage (0-100%)
uint8_t readPercentage();          // 0-100%

// ADC Configuration
uint8_t getAdcResolution();        // Get current resolution bits
void setAdcResolution(uint8_t res); // Set resolution (8,10,12 bits)

// Cleanup
bool end();
```

#### Resolution Comparison
| Resolution | Max Value | Precision | Use Case |
|-----------|-----------|-----------|----------|
| 8-bit | 255 | Low | Speed-critical apps |
| 10-bit | 1023 | Medium | Most applications |
| 12-bit | 4095 | High (default) | Precision sensing |

---

### Class: LF_Alpha_rgbLEDs (NeoPixel WS2812B)

#### Methods

```cpp
LF_Alpha_rgbLEDs leds(5);           // 5 NeoPixels on GPIO48

// Initialization
bool begin();

// Display Control
void write(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void write(uint16_t n, uint32_t color);  // 32-bit color
void clear();
void show();

// Effects
void blink(uint8_t noOfLED, uint8_t r, uint8_t g, uint8_t b, uint16_t duration = 1000);
void rainbow(uint16_t firstHue = 0, int8_t reps = 1, uint8_t saturation = 255, uint8_t brightness = 255);

// Configuration
void setBrightness(uint8_t value);   // 0-255
uint16_t numPixels();
uint32_t getPixelColor(uint16_t n);

// Cleanup
bool end();
```

#### Color Format
```cpp
// RGB values (0-255 each)
leds.write(0, 255, 0, 0);      // Pixel 0 = Red
leds.write(1, 0, 255, 0);      // Pixel 1 = Green
leds.write(2, 0, 0, 255);      // Pixel 2 = Blue

// 32-bit color (0xRRGGBB)
leds.write(3, 0xFF00FF);       // Magenta
leds.show();                   // Update display
```

---

### Class: LF_Alpha_MAX30102

#### Methods

```cpp
LF_Alpha_MAX30102 heartRate;

// Initialization
bool begin(uint8_t i2cAddr = 0x57);

// Configuration
void configure(uint8_t ledBrightness = 0x1F, uint8_t sampleAverage = SAMPLEAVG4);
void setADCRange(uint8_t adcRange);
void setSampleRate(uint8_t sampleRate);  // 50-3200 Hz

// Reading Data
void readSPO2(int32_t &SPO2, int8_t &SPO2Valid,
              int32_t &heartRate, int8_t &heartRateValid);
uint32_t readBPM();            // Beats per minute
float readTempC();             // Temperature in Celsius
float readTempF();             // Temperature in Fahrenheit

// Cleanup
bool end();
```

#### Data Interpretation
| Variable | Valid Range | Meaning |
|----------|------------|---------|
| SPO2 | 0-100% | Blood oxygen saturation |
| SPO2Valid | 0/1 | Data valid flag |
| heartRate | 30-200 bpm | Beats per minute |
| heartRateValid | 0/1 | Data valid flag |

---

### Class: LF_Alpha_RTC (Real-Time Clock)

#### Methods

```cpp
LF_Alpha_RTC rtc;

// Initialization
bool begin(uint8_t clockAddress = 0x68);

// Time Setting
void setDateTime(int16_t year, int8_t month, int8_t day,
                 int8_t hour, int8_t min, int8_t sec);
void setTime12(uint8_t hour, uint8_t min, uint8_t sec, bool isPM);
void setClockMode(bool h12);          // true=12h, false=24h

// Reading
DateTime now();
String readTime();                    // Returns "HH:MM:SS"
String readDate();                    // Returns "DD/MM/YYYY"

// Information
uint8_t getDayNumber();               // 0-6 (Sun-Sat)
String getDayName();
String getMonthName();
uint8_t getMonthNumber();
uint16_t getYear();
bool isLeapYear(int16_t year);

// Cleanup
bool end();
```

---

### Class: LF_Alpha_OLED (SSD1306 Display)

#### Methods

```cpp
LF_Alpha_OLED display;                // 128x64 pixels

// Initialization
bool begin(uint8_t I2CAddress = 0x3C);

// Drawing Functions
void clearDisplay();
void display();                       // Update screen
void drawPixel(int16_t x, int16_t y);
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h);
void drawCircle(int16_t x, int16_t y, int16_t r);
void fillCircle(int16_t x, int16_t y, int16_t r);
void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);

// Text Functions
void setCursor(int16_t x, int16_t y);
void setTextSize(uint8_t size);       // 1-10
void setTextColor(uint16_t color);    // WHITE, BLACK, INVERSE
void print(const char *text);
void println(const char *text);

// Cleanup
bool end();
```

---

## Hardware Configuration

### GPIO Pin Mapping (ESP32-S3 Mini-1)

```
Communication (I2C):
  SDA    ->  GPIO 38
  SCL    ->  GPIO 37

Sensors (I2C Address on SDA/SCL):
  MPU6050      ->  0x69 (Interrupt: GPIO16)
  MAX30102     ->  0x57 (Interrupt: GPIO17)
  AHT20        ->  0x38
  APDS9960     ->  0x39 (Interrupt: GPIO15)
  DS3231 RTC   ->  0x68

Output/Control:
  Buzzer       ->  GPIO 18
  LED1         ->  GPIO 21 (Active LOW)
  LED2         ->  GPIO 26 (Active HIGH)
  Servo        ->  GPIO 47 (PWM)
  Relay1       ->  GPIO 36
  Relay2       ->  GPIO 35
  NeoPixel     ->  GPIO 48

Input:
  Up Button    ->  GPIO 13
  Down Button  ->  GPIO 14
  Back Button  ->  GPIO 11
  OK Button    ->  GPIO 12
  Echo (HC-SR04) ->  GPIO 34
  Trigger (HC-SR04) ->  GPIO 33
  Potentiometer  ->  GPIO 1 (ADC)
```
---

## Third-Party Libraries

This project integrates the following open-source components:

| Library | Author | License | Purpose |
|---------|--------|---------|---------|
| MPU6050 / I2Cdev | Jeff Rowberg / ElectronicsCats | MIT | 6-axis motion sensor |
| Adafruit_SSD1306 | Adafruit Industries | BSD | OLED display driver |
| Adafruit_NeoPixel | Adafruit Industries | LGPL-3.0 | RGB LED control |
| Adafruit_APDS9960 | Adafruit Industries | MIT | Color/gesture sensor |
| AHT20 | dvarrel | CC0-1.0 | Temperature/humidity |
| DS3231-RTC | hasenradball | MIT | Real-time clock |
| DFRobot_MAX30102 | DFRobot | MIT | Heart rate sensor |
| ESP32Servo | Kevin Harrington | LGPL-2.1 | Servo control |

**License Compliance:** See individual LICENSE.txt files in src/lib/[LibraryName]/

---

## License

**LF_Alpha Library:** Copyright (c) 2025 EARTKEY

Licensed under the **MIT License** - See LICENSE file for details.

**Third-Party Components:** Each component retains its original license. Complete attribution available in NOTICE.md

---

## Contact Support

- **Author:** EARTKEY
- **Repository:** https://github.com/EARTKEY/LF_Alpha
- **GitHub Issues:** Report Issues
- **Email:** eartkey@gmail.com

---