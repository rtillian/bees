#include "ir_obstacle_sim.h"

IRObstacle_sim::IRObstacle_sim(uint8_t pin) : _pin(pin) {}

void IRObstacle_sim::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = HIGH;
    Serial.printf("Sim-IR Obstacle Module gestartet (HIGH)\r\n");
}

void IRObstacle_sim::update() {
    // Gibt eine Zahl zwischen 0 und 12 aus. Sonst nix.
    bool currentState = !_lastState;
    Serial.println("Pseudo - update ir_obstacle_sim.cpp");
    delay(1000);
    int zufall = random(12);
    _count = zufall;
    _lastState = currentState;
}

long IRObstacle_sim::getCountIn() const {
    return _count;
}
long IRObstacle_sim::getCountOut() const {
    return _count;
}
void IRObstacle_sim::resetCount() {
    _count = 0;
}