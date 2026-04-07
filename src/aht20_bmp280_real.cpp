#include <aht20_bmp280_real.h>
// aht20_bmp280.cpp

AHT20_BMP280::AHT20_BMP280() {
    // Konstruktor – hier noch nichts machen
}

bool AHT20_BMP280::begin() {
    Serial.println("Initialisiere AHT20 + BMP280...");

    Wire.begin();                     // Standard-Pins: SDA=21, SCL=22

    // AHT20 starten
    if (!aht.begin()) {
        Serial.println("Fehler: AHT20 nicht gefunden!");
        return false;
    }

    // BMP280 starten (versucht zuerst 0x76, dann 0x77)
    if (!bmp.begin(0x76)) {
        if (!bmp.begin(0x77)) {
            Serial.println("Fehler: BMP280 nicht gefunden!");
            return false;
        }
    }

    // BMP280 Sampling-Einstellungen optimieren
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);

    initialized = true;
    Serial.println("AHT20 + BMP280 erfolgreich initialisiert!");
    return true;
}

bool AHT20_BMP280::update() {
    if (!initialized) {
        return false;
    }

    sensors_event_t hum, temp;
    aht.getEvent(&hum, &temp);

    temperatureAHT = temp.temperature;
    humidity = hum.relative_humidity;

    temperatureBMP = bmp.readTemperature();
    pressure = bmp.readPressure() / 100.0;           // hPa
    altitude = bmp.readAltitude(1013.25);            // Standard-Luftdruck am Meeresspiegel

    return true;
}

// Getter-Methoden
float AHT20_BMP280::getTemperatureAHT() const { return temperatureAHT; }
float AHT20_BMP280::getHumidity() const { return humidity; }
float AHT20_BMP280::getTemperatureBMP() const { return temperatureBMP; }
float AHT20_BMP280::getPressure() const { return pressure; }
float AHT20_BMP280::getAltitude() const { return altitude; }

void AHT20_BMP280::printValues() const {
    Serial.println("=== AHT20 + BMP280 Werte ===");
    Serial.printf("Temperatur AHT20 : %.2f °C\n", temperatureAHT);
    Serial.printf("Luftfeuchtigkeit : %.2f %% \n", humidity);
    Serial.printf("Temperatur BMP280: %.2f °C\n", temperatureBMP);
    Serial.printf("Luftdruck        : %.2f hPa\n", pressure);
    Serial.printf("Höhe (ca.)       : %.1f m\n", altitude);
    Serial.println("=============================");
}
