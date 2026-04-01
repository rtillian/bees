#include "inmp441_mic_sim.h"

INMP441Mic_sim::INMP441Mic_sim() : FFT(vReal, vImag, BUFFER_SIZE, 22050) {}

bool INMP441Mic_sim::begin() {
    Serial.println("Initialisiere INMP441 (Legacy I2S)...");

    initialized = true;
    lastLoudnessTime = millis();
    lastFrequencyTime = millis();

    Serial.println("INMP441 bereit.\n");
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
    Serial.println("isNewLoudnessReady");
    return (millis() - lastLoudnessTime >= LOUDNESS_INTERVAL);
}

bool INMP441Mic_sim::isNewFrequencyReady() const {
    Serial.println("isNewFrequeceReady");
    return (millis() - lastFrequencyTime >= FREQUENCY_INTERVAL);
}

float INMP441Mic_sim::getAverage_dB() const {
    float avgRMS = 13.2; //(volumeCount > 0) ? (sumVolume / volumeCount) : 0.0;
    Serial.println("getAverageDb");
    return convertTo_dB(avgRMS);
}

float INMP441Mic_sim::getDominantFrequency() const {
    Serial.println("getDominantFrequency");
    return lastDominantFreq;
}

float INMP441Mic_sim::calculateRMS(const int32_t* samples, size_t count) {
    Serial.println("calculateRMS");
    return 32.2;
}

float INMP441Mic_sim::convertTo_dB(float rms) const {
    Serial.println("convertTo_dB");
    return 66.2;

}