#include "aht20_bmp280_sim.h"

AHT20_BMP280_sim::AHT20_BMP280_sim() {
    // leer
}

bool AHT20_BMP280_sim::begin() {
    Serial.println("SIMULATION - Initialisiere AHT20 + BMP280...");
    initialized = true;
    return true;
}

// Neue Funktion: Liest alle Werte aus und gibt sie als Struktur zurück
SensorValues AHT20_BMP280_sim::hole_werte() {
    SensorValues values = {0.0, 0.0, 0.0, 0.0, 0.0};  // Standardwerte

    if (!initialized) {
        return values;
    }

    //sensors_event_t hum, temp;
    //aht.getEvent(&hum, &temp);

    values.tempAHT   = 99.9;
    values.humidity  = 88.8; 
    values.tempBMP   = 77.7;
    values.pressure  = 66.6; 
    values.altitude  = 55.5; 

    return values;
}

// Einzelne Getter (optional)
float AHT20_BMP280_sim::getTemperatureAHT() const { return 0.0; } // Platzhalter, falls benötigt
float AHT20_BMP280_sim::getHumidity() const { return 0.0; }