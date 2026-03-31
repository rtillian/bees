#include "ir_obstacle.h"

IRObstacle::IRObstacle(uint8_t pin) : _pin(pin) {}

void IRObstacle::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin);
    Serial.printf("IR Obstacle Module gestartet (Pin %d)\n", _pin);
}

void IRObstacle::update() {
    bool currentState = digitalRead(_pin);
    unsigned long now = millis();

    // Nur zählen, wenn:
    // 1. Flanke von HIGH → LOW erkannt wird
    // 2. Seit dem letzten Zählen mindestens "minGap" Millisekunden vergangen sind
    if (_lastState == HIGH && currentState == LOW) {
        if (now - _lastDetectionTime > _minGap) {
            _count++;
            _lastDetectionTime = now;
        }
    }

    _lastState = currentState;
}

long IRObstacle::getCount() const {
    return _count;
}

void IRObstacle::resetCount() {
    _count = 0;
}