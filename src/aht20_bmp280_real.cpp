// aht20_bmp280.cpp
#include "aht20_bmp280_real.h"

AHT20_BMP280::AHT20_BMP280() {
    // leer
}

bool AHT20_BMP280::begin() {
    Serial.println("Initialisiere AHT20 + BMP280...");

    Wire.begin();

    if (!aht.begin()) {
        Serial.println("Fehler: AHT20 nicht gefunden!");
        return false;
    }

    if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
        Serial.println("Fehler: BMP280 nicht gefunden!");
        return false;
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);

    initialized = true;
    Serial.println("AHT20 + BMP280 erfolgreich initialisiert!");
    return true;
}

// Neue Funktion: Liest alle Werte aus und gibt sie als Struktur zurück
SensorValues AHT20_BMP280::hole_werte() {
    SensorValues values = {0.0, 0.0, 0.0, 0.0, 0.0};  // Standardwerte

    if (!initialized) {
        return values;
    }

    sensors_event_t hum, temp;
    aht.getEvent(&hum, &temp);

    values.tempAHT   = temp.temperature;
    values.humidity  = hum.relative_humidity;
    values.tempBMP   = bmp.readTemperature();
    values.pressure  = bmp.readPressure() / 100.0;      // hPa
    values.altitude  = bmp.readAltitude(1013.25);

    return values;
}

// Einzelne Getter (optional)
float AHT20_BMP280::getTemperatureAHT() const { return 0.0; } // Platzhalter, falls benötigt
float AHT20_BMP280::getHumidity() const { return 0.0; }