#include "inmp441_mic_sim.h"

INMP441Mic_sim::INMP441Mic_sim() : FFT(vReal, vImag, BUFFER_SIZE, 22050) {}

bool INMP441Mic_sim::begin() {
    Serial.println("Initialisiere INMP441 (Legacy I2S)...");

    initialized = true;
    lastLoudnessTime = millis();
    lastFrequencyTime = millis();

    return true;
}

void INMP441Mic_sim::update() {
    if (!initialized) return;
    unsigned long now = millis();

    // Lautstärke-Timer aktualisieren
    if (now - lastLoudnessTime >= LOUDNESS_INTERVAL) {
        lastLoudnessTime = now;
        Serial.println("Pseudo - update inmp441_mic_sim.cpp");
    }
}

bool INMP441Mic_sim::isNewLoudnessReady() const {
    return (millis() - lastLoudnessTime >= LOUDNESS_INTERVAL);
}

bool INMP441Mic_sim::isNewFrequencyReady() const {
    return (millis() - lastFrequencyTime >= FREQUENCY_INTERVAL);
}

float INMP441Mic_sim::getAverage_volume() const {
    float avgRMS = 13.2; //(volumeCount > 0) ? (sumVolume / volumeCount) : 0.0;
    return 22.2;
}

float INMP441Mic_sim::getDominantFrequency() const {
    return 33;
}

float INMP441Mic_sim::calculateRMS(const int32_t* samples, size_t count) {
    return 32.2;
}

float INMP441Mic_sim::convertTo_volume(float rms) const {
    return 66.2;
}