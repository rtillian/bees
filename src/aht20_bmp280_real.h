// aht20_bmp280.h
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

class AHT20_BMP280 {
private:
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;

    // Gemessene Werte
    float temperatureAHT = 0.0;
    float humidity = 0.0;
    float temperatureBMP = 0.0;
    float pressure = 0.0;
    float altitude = 0.0;

    bool initialized = false;

public:
    AHT20_BMP280();                    // Konstruktor

    bool begin();                      // Initialisierung (I²C + Sensoren)
    bool update();                     // Neue Werte von Sensoren lesen

    // Getter-Methoden für die Werte
    float getTemperatureAHT() const;
    float getHumidity() const;
    float getTemperatureBMP() const;
    float getPressure() const;         // in hPa
    float getAltitude() const;         // in Metern (bezogen auf 1013.25 hPa)

    // Zusätzliche praktische Methode
    void printValues() const;          // Ausgabe aller Werte auf Serial
};