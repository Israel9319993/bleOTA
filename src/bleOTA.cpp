#include "bleOTA.h"

// --- Internal NimBLE Callbacks ---
class bleOTAServerCallbacks : public NimBLEServerCallbacks {
    void onDisconnect(NimBLEServer* pServer) {
        NimBLEDevice::startAdvertising(); // Always stay discoverable
    }
};

class bleOTADataCallbacks : public NimBLECharacteristicCallbacks {
    bleOTA* _inst;
public:
    bleOTADataCallbacks(bleOTA* inst) : _inst(inst) {}
    void onWrite(NimBLECharacteristic* pChar, NimBLEConnInfo& connInfo) {
        _inst->_handleData(pChar->getValue());
    }
};

// --- bleOTA Class Implementation ---

bleOTA::bleOTA() : _pServer(nullptr), _onProgress(nullptr) {}

void bleOTA::begin(const char* deviceName) {
    NimBLEDevice::init(deviceName);

    _pServer = NimBLEDevice::createServer();
    _pServer->setCallbacks(new bleOTAServerCallbacks());

    NimBLEService* pService = _pServer->createService(BLE_OTA_SERVICE_UUID);

    // Single characteristic for both Start JSON and Binary chunks
    NimBLECharacteristic* pChar = pService->createCharacteristic(
        BLE_OTA_CHAR_UUID, 
        NIMBLE_PROPERTY::WRITE
    );
    pChar->setCallbacks(new bleOTADataCallbacks(this));

    pService->start();
    NimBLEAdvertising* pAdv = NimBLEDevice::getAdvertising();
    pAdv->addServiceUUID(BLE_OTA_SERVICE_UUID);
    pAdv->start();
}

void bleOTA::_handleData(const std::string& data) {
    // 1. Check for OTA Start Command (JSON)
    if (!_otaInProgress) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, data);

        if (!error && doc["type"] == "ota_start") {
            _expectedSize = doc["size"];
            const char* md5 = doc["md5"];

            if (md5 && strlen(md5) == 32) {
                strncpy(_expectedMD5, md5, 32);
                _expectedMD5[32] = '\0';

                Update.setMD5(_expectedMD5);
                if (Update.begin(_expectedSize)) {
                    _otaInProgress = true;
                    _receivedSize = 0;
                    Serial.printf("🚀 OTA Started. Size: %u, MD5: %s\n", _expectedSize, _expectedMD5);
                }
            }
        }
        return;
    }

    // 2. Handle Binary Chunks
    size_t len = data.length();
    size_t written = Update.write((uint8_t*)data.data(), len);
    _receivedSize += written;

    // Trigger Progress Callback
    if (_onProgress) {
        int percent = (_receivedSize * 100) / _expectedSize;
        _onProgress(percent);
    }

    // 3. Finalize Update
    if (_receivedSize >= _expectedSize) {
        if (Update.end(true)) {
            if (Update.md5String().equalsIgnoreCase(_expectedMD5)) {
                Serial.println("✅ OTA Success! Rebooting...");
                delay(1000);
                ESP.restart();
            } else {
                Serial.println("❌ MD5 Mismatch!");
            }
        } else {
            Serial.printf("❌ Update Error: %s\n", Update.errorString());
        }
        _otaInProgress = false;
    }
}