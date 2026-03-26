#include <bleOTA.h>

bleOTA ota;

void setup() {
    Serial.begin(115200);

    // Initialize BLE with a specific name
    ota.begin("GoNav_Update_Tool");

    // Optional: Print progress to Serial (or update an LCD/LEDs)
    ota.onProgress([](int progress) {
        Serial.printf("Download Progress: %d%%\n", progress);
    });

    Serial.println("BLE OTA Service is active and advertising...");
}

void loop() {
    // Main loop remains empty or handles other non-BLE logic
    delay(1000);
}