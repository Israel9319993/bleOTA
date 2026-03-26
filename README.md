# bleOTA
Here is a professional and "sweet" documentation setup for your **bleOTA** library. You can copy this directly into your GitHub repository.

### 📝 Repository Description
> ⚡ A lightweight, secure, and developer-friendly ESP32 library for Over-The-Air (OTA) firmware updates via Bluetooth Low Energy (BLE). Features integrated MD5 verification and NimBLE support.

---

### 📂 File Structure (for reference)
Ensure your GitHub repo looks like this:
```text
bleOTA/
├── src/
│   ├── bleOTA.h
│   └── bleOTA.cpp
├── examples/
│   └── SimpleOTA/
│       └── SimpleOTA.ino
├── library.properties
├── README.md
└── LICENSE
```

---

### 📄 Proposed README.md

# 📶 bleOTA: BLE Firmware Updates for ESP32

**bleOTA** is a focused, high-performance Arduino library designed to handle firmware updates over Bluetooth Low Energy. Unlike other bulky libraries, **bleOTA** does exactly one thing: provides a secure, reliable bridge between your mobile app and the ESP32's flash memory.

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Framework: Arduino](https://img.shields.io/badge/Framework-Arduino-blue.svg)](https://www.arduino.cc/)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)

---

## ✨ Features
- 🚀 **Fast Transfers**: Optimized for binary chunk handling via NimBLE.
- 🛡️ **Secure**: Integrated **MD5 Checksum** verification to prevent corrupted firmware from flashing.
- 📦 **Minimalist**: Focuses strictly on OTA logic—no extra "bloat" from WiFi or configuration modules.
- 🧩 **Callback-Driven**: Easily hook into the update progress to update your LEDs, LCDs, or Serial logs.
- 🔄 **Auto-Advertising**: Automatically restarts BLE advertising on disconnect.

---

## 🚀 Quick Start

### 1. Installation
1. Download this repository as a `.zip`.
2. In Arduino IDE, go to **Sketch** -> **Include Library** -> **Add .ZIP Library**.
3. **Requirement**: You must have the [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) and [ArduinoJson](https://github.com/bblanchon/ArduinoJson) libraries installed.

### 2. Basic Usage
```cpp
#include <bleOTA.h>

bleOTA ota;

void setup() {
    Serial.begin(115200);

    // Initialize the BLE OTA service with your device name
    ota.begin("My_ESP32_Device");

    // Hook into the progress callback (0 - 100%)
    ota.onProgress([](int progress) {
        Serial.printf("⚡ Update Progress: %d%%\n", progress);
    });

    Serial.println("Waiting for BLE OTA connection...");
}

void loop() {
    // No code needed here for OTA!
}
```

---

## 📱 Data Protocol (For App Developers)

The library expects data on the `beb5483f-36e2-4688-b7f5-ea07361b26a9` characteristic in two stages:

### Stage 1: The Start Packet (JSON)
Before sending binary data, send a JSON object to initialize the update:
```json
{
  "type": "ota_start",
  "size": 1450280,
  "md5": "59376cf7948b618039c46e2f21647a15",
  "version": "v1.0.2"
}
```

### Stage 2: The Binary Stream
Immediately follow with the raw bytes of your `.bin` file. The library writes these chunks directly to the ESP32 OTA partition. Once the `size` defined in Stage 1 is reached, the library verifies the MD5 and reboots.

---

## 🛠️ Hardware Requirements
- **Microcontroller**: ESP32, ESP32-S3, ESP32-C3.
- **Partition Scheme**: You must select a partition scheme that includes **OTA** (e.g., "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)").

---

## 🔗 Technical Details
- **Service UUID**: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`
- **Characteristic UUID**: `beb5483f-36e2-4688-b7f5-ea07361b26a9`
- **MTU Size**: It is recommended to request a higher MTU (e.g., 512) from your mobile app for faster transfer speeds.

---

## 🤝 Contributing
Contributions are welcome! If you find a bug or have a feature request, please open an issue or submit a pull request.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
**Developed with ❤️ for the ESP32 Community.**

---

### 📝 `library.properties`
Put this in the root folder so the Arduino IDE recognizes it:
```ini
name=bleOTA
version=1.0.0
author=YourName
maintainer=YourName
sentence=Simplified BLE OTA Firmware updates for ESP32.
paragraph=A focused library for updating ESP32 firmware via Bluetooth Low Energy using NimBLE. Includes MD5 verification and progress callbacks.
category=Communication
url=https://github.com/yourusername/bleOTA
architectures=esp32
includes=bleOTA.h
depends=NimBLE-Arduino, ArduinoJson
```
