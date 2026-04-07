#ifndef AHT20_BMP280_H
#define AHT20_BMP280_H

#include <Arduino.h>
#include <driver/i2s.h>
#include <arduinoFFT.h>


class AHT20_BMP280_sim {
public:
    AHT20_BMP280_sim();
    bool begin();
    void update();                          // Muss regelmäßig aufgerufen werden

    bool isNewLoudnessReady() const;        // alle 2 Sekunden
    bool isNewFrequencyReady() const;       // alle 5 Sekunden
    float getAverage_volume() const;
    float getDominantFrequency() const;

private:
    float calculateRMS(const int32_t* samples, size_t count);
    float convertTo_dB(float rms) const;

    // I2S
    static constexpr int BUFFER_SIZE = 512;     // Größer für gute FFT
    int32_t samples[BUFFER_SIZE];

    // Lautstärke (gleitender Mittelwert)
    static constexpr int LOUDNESS_AVG_SIZE = 40;
    float volumeBuffer[LOUDNESS_AVG_SIZE] = {0};
    int   volumeIndex = 0;
    float sumVolume = 0.0;
    int   volumeCount = 0;

    // FFT für Tonhöhe
    float vReal[BUFFER_SIZE];
    float vImag[BUFFER_SIZE];
    ArduinoFFT<float> FFT;

    // Timing
    unsigned long lastLoudnessTime = 0;
    unsigned long lastFrequencyTime = 0;
    const unsigned long LOUDNESS_INTERVAL = 3000;   // 2 Sekunden
    const unsigned long FREQUENCY_INTERVAL = 3000;  // 5 Sekunden
    

    float dcOffset = 0.0;
    float lastDominantFreq = 0.0;
    bool  initialized = false;
};

#endif