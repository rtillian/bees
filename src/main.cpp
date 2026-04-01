#include <Arduino.h>
#include "inmp441_mic.h"
#include "ir_obstacle.h"
#include "myUtils.h"
#include "supabase_client.h"
#include "data_buffer.h"

INMP441Mic mic;
IRObstacle irSensor(26);
MyUtils utils;
SupabaseClient supabase;
DataBuffer dataBuffer;

// WiFi Zugangsdaten 
const char* WIFI_SSID = "A1_19FC_Rudolf";
const char* WIFI_PASS = "till3333";

unsigned long lastPrintTime = 0;
unsigned long lastUploadTime = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("   INMP441 + IR Bienenzähler + Supabase");
    Serial.println("========================================\n");

    utils.printDeviceInfo();

    mic.begin();
    irSensor.begin();

    if (supabase.begin(WIFI_SSID, WIFI_PASS)) {
        Serial.println("Supabase-Client bereit.");
    }

    lastPrintTime = millis();
    lastUploadTime = millis();
}

void loop() {
    mic.update();
    irSensor.update();

    unsigned long now = millis();

    // Lokale Anzeige alle 5 Sekunden
    if (now - lastPrintTime >= 5000) {
        float dB = mic.getAverage_dB();
        float freq = mic.getDominantFrequency();
        long countIn = irSensor.getCountIn();      // <-- neu
        long countOut = irSensor.getCountOut();    // <-- neu

        float temperature = 0.0;   // später mit Sensor füllen
        float humidity = 0.0;      // später mit Sensor füllen
        float co2 = 0.0;           // später mit Sensor füllen
        float frequency = 0.0;   // später mit Sensor füllen
        unsigned long beeCountIn = 5;   // später mit Sensor füllen
        unsigned long beeCountOut = 5;   // später mit Sensor füllen


        Serial.printf("📊 %5.1f dB    |    ♪ %6.0f Hz    |    In: %ld  Out: %ld\n", 
                      dB, freq, countIn, countOut);

        // Daten in den Puffer speichern
        dataBuffer.addData(
            "abc",
            3, 
            temperature, 
            humidity, 
            co2, 
            dB, 
            frequency, 
            countIn, 
            countOut
        );

        lastPrintTime = now;
    }

    // Alle 10 Minuten Puffer an Supabase senden
    if (now - lastUploadTime >= 600000) { //600000) {        // 10 Minuten
        Serial.println("--- Starte Upload an Supabase (sensor_data) ---");
        dataBuffer.uploadBuffer(supabase);
        lastUploadTime = now;
    }

    delay(10);
}