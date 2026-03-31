#include "dht22_sensor.h"
#include <DHT.h>

#define DHTTYPE DHT22

DHT dht_sensor;   // Globale DHT-Instanz

DHT22Sensor::DHT22Sensor(uint8_t pin) 
    : _pin(pin), _temperature(NAN), _humidity(NAN), _lastReadTime(0) {
    dht_sensor = DHT(_pin, DHTTYPE);
}

void DHT22Sensor::begin() {
    dht_sensor.begin();
    Serial.println("DHT22 Sensor initialisiert");
    delay(2000);                    // DHT22 braucht Startzeit
}

bool DHT22Sensor::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - _lastReadTime < 2000) {   // max. alle 2 Sekunden
        return false;
    }

    float h = dht_sensor.readHumidity();
    float t = dht_sensor.readTemperature();

    if (isnan(h) || isnan(t)) {
        // Serial.println("DHT22: Lesefehler");
        return false;
    }

    _humidity = h;
    _temperature = t;
    _lastReadTime = currentMillis;
    return true;
}

float DHT22Sensor::getTemperature() const {
    return _temperature;
}

float DHT22Sensor::getHumidity() const {
    return _humidity;
}

void DHT22Sensor::printValues() const {
    Serial.printf("🌡️  Temp: %.1f °C    |    💧 Feuchtigkeit: %.1f %%\n", 
                  _temperature, _humidity);
}