#include <Arduino.h>
//#include "inmp441_mic.h"
#include "inmp441_mic.h"
#include "ir_obstacle.h"
#include "myUtils.h"
#include "supabase_client.h"
#include "data_buffer.h"

//INMP441Mic mic;
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

bool isRunning = true;

void setup() {
    Serial.begin(115200);
    delay(1000);
    #if MIC_IS_SIMULATION
        Serial.println("→ INMP441 läuft im Simulations-Modus (Pseudo-Werte)");
    #else
        Serial.println("→ INMP441 läuft mit echter Hardware");
    #endif
    delay(5000);
    
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

    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();                    // entfernt Leerzeichen, \r, \n

        if (command.equalsIgnoreCase("stop")) {
            isRunning = false;
            Serial.println(">>> Loop angehalten (gestoppt)");
        }
        else if (command.equalsIgnoreCase("resume")) {
            isRunning = true;
            Serial.println(">>> Loop fortgesetzt (resumed)");
        }
        else if (command.length() > 0) {
            Serial.print("Unbekannter Befehl: ");
            Serial.println(command);
        }
    }

    // === 2. Nur ausführen, wenn isRunning == true ===
    if (!isRunning) {
        delay(100);          // kleine Pause, damit der ESP32 nicht den Watchdog triggert
        return;             // überspringt den Rest des loop()
    }
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


        Serial.printf("📊 %5.1f dB    |    ♪ %6.0f Hz    |    In: %ld  Out: %ld\r\n", 
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
    if (now - lastUploadTime >= 6000) { //600000) {        // 10 Minuten
        Serial.println("--- Starte Upload an Supabase (sensor_data) ---");
        dataBuffer.uploadBuffer(supabase);
        lastUploadTime = now;
    }

    delay(10);
}