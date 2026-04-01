#include "myUtils.h"

MyUtils::MyUtils() {
    // Konstruktor kann leer bleiben
}

String MyUtils::getDeviceID() {
    // WiFi Interface aktivieren, falls noch nicht geschehen
    if (WiFi.getMode() == WIFI_OFF) {
        WiFi.mode(WIFI_STA);
    }
    
    // Warte, bis die MAC-Adresse verfügbar ist
    unsigned long startTime = millis();
    while (WiFi.macAddress() == "00:00:00:00:00:00") {
        delay(20);
        if (millis() - startTime > 1000) {   // Timeout nach 1 Sekunde
            Serial.println("Warnung: Konnte MAC-Adresse nicht lesen!");
            return "UNKNOWN";
        }
    }

    String mac = WiFi.macAddress();
    //mac.replace(":", "");
    
    deviceID = mac;
    return deviceID;
}

void MyUtils::printDeviceInfo() {
    Serial.println("\n=== Geräte-Informationen ===");
    Serial.printf("MAC-Adresse: %s\r\n", getDeviceID().c_str());
    Serial.printf("ESP32 Chip ID: %08X\r\n", (uint32_t)ESP.getEfuseMac());
    Serial.printf("CPU-Frequenz: %d MHz\r\n", ESP.getCpuFreqMHz());
    Serial.println("===========================\n");
}