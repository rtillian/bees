#ifndef INMP441_MIC_H
#define INMP441_MIC_H

#include <Arduino.h>
#include <driver/i2s.h>

class INMP441Mic {
public:
    INMP441Mic();
    void begin();
    float getSoundLevel();           // Gibt RMS-Lautstärke zurück
    void printSoundLevel() const;

private:
    float calculateRMS(const int32_t* samples, size_t count);
    
    static constexpr int BUFFER_SIZE = 64;
    int32_t samples[BUFFER_SIZE];
};

#endif