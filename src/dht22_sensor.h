#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include <Arduino.h>

class DHT22Sensor {
public:
    DHT22Sensor(uint8_t pin);
    void begin();
    bool update();                  // Liest neue Werte (mit Timing)
    float getTemperature() const;
    float getHumidity() const;
    void printValues() const;

private:
    uint8_t _pin;
    float _temperature;
    float _humidity;
    unsigned long _lastReadTime;
};

#endif