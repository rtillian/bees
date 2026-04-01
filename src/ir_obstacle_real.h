#ifndef IR_OBSTACLE_H
#define IR_OBSTACLE_H

#include <Arduino.h>

class IRObstacle {
public:
    IRObstacle(uint8_t pin);
    void begin();
    void update();
    long getCountIn() const;
    long getCountOut() const;
    void resetCount();

private:
    uint8_t _pin;
    long _count = 0;
    bool _lastState = HIGH;
    unsigned long _lastDetectionTime = 0;
    const unsigned long _minGap = 30;     // ← Hier kannst du einstellen!
};

#endif