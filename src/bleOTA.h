#ifndef BLE_OTA_H
#define BLE_OTA_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <ArduinoJson.h>
#include <Update.h>
#include <functional>

// Standard UUIDs for the OTA Service
#define BLE_OTA_SERVICE_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_OTA_CHAR_UUID     "beb5483f-36e2-4688-b7f5-ea07361b26a9"

class bleOTA {
public:
    // Callback to update your UI or LEDs during download
    typedef std::function<void(int)> ProgressCallback;

    bleOTA();

    /**
     * @param deviceName Name appearing in Bluetooth scan
     */
    void begin(const char* deviceName);

    /**
     * @param cb Function that receives progress (0-100)
     */
    void onProgress(ProgressCallback cb) { _onProgress = cb; }

private:
    bool _otaInProgress = false;
    size_t _expectedSize = 0;
    size_t _receivedSize = 0;
    char _expectedMD5[33];

    NimBLEServer* _pServer;
    ProgressCallback _onProgress;

    void _handleData(const std::string& data);

    friend class bleOTAServerCallbacks;
    friend class bleOTADataCallbacks;
};

#endif