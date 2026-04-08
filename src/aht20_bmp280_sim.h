#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

// Struktur für alle Sensorwerte
struct SensorValues {
    float tempAHT;      // Temperatur AHT20
    float humidity;     // Luftfeuchtigkeit
    float tempBMP;      // Temperatur BMP280
    float pressure;     // Luftdruck in hPa
    float altitude;     // Höhe in Metern
};

class AHT20_BMP280_sim {
private:
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;

    bool initialized = false;

public:
    AHT20_BMP280_sim();
    bool begin();                    // Initialisierung
    SensorValues hole_werte();       // Liest Sensoren aus und gibt alle Werte zurück

    // Einzelne Getter (falls du sie später trotzdem brauchst)
    float getTemperatureAHT() const;
    float getHumidity() const;
    // ... weitere Getter bei Bedarf
};