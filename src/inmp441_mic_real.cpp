#include "inmp441_mic_real.h"

INMP441Mic::INMP441Mic() : FFT(vReal, vImag, BUFFER_SIZE, 22050) {}

bool INMP441Mic::begin() {
    Serial.println("Initialisiere INMP441 (Legacy I2S)...");

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 22050,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 32,
        .ws_io_num = 25,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = 33
    };

    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.printf("i2s_driver_install Fehler: %d\n", err);
        return false;
    }

    err = i2s_set_pin(I2S_NUM_0, &pin_config);
    if (err != ESP_OK) {
        Serial.printf("i2s_set_pin Fehler: %d\n", err);
        return false;
    }

    i2s_set_clk(I2S_NUM_0, 22050, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    // DC-Offset kalibrieren
    Serial.println("Kalibriere DC-Offset bei Stille...");
    float sum = 0.0;
    for (int i = 0; i < 120; i++) {
        size_t bytesRead = 0;
        i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);
        if (bytesRead > 0) {
            size_t count = bytesRead / sizeof(int32_t);
            sum += calculateRMS(samples, count);
        }
        delay(12);
    }
    dcOffset = sum / 120;
    Serial.printf("DC-Offset: %.0f\n", dcOffset);

    initialized = true;
    lastLoudnessTime = millis();
    lastFrequencyTime = millis();

    Serial.println("INMP441 bereit.\n");
    return true;
}

void INMP441Mic::update() {
    if (!initialized) return;

    // --- I2S Messung ---
    size_t bytesRead = 0;
    i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);

    if (bytesRead == 0) return;

    size_t count = bytesRead / sizeof(int32_t);
    float rawRMS = calculateRMS(samples, count);
    float corrected = max(0.0f, rawRMS - dcOffset);

    // Gleitender Mittelwert für Lautstärke
    sumVolume -= volumeBuffer[volumeIndex];
    volumeBuffer[volumeIndex] = corrected;
    sumVolume += corrected;
    volumeIndex = (volumeIndex + 1) % LOUDNESS_AVG_SIZE;

    if (volumeCount < LOUDNESS_AVG_SIZE) volumeCount++;

    // Timing
    unsigned long now = millis();

    // FFT für Tonhöhe alle 5 Sekunden
    if (now - lastFrequencyTime >= FREQUENCY_INTERVAL) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            vReal[i] = (i < count) ? (float)((int16_t)samples[i]) : 0.0f;
            vImag[i] = 0.0f;
        }

        FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.compute(FFT_FORWARD);
        FFT.complexToMagnitude();

        lastDominantFreq = FFT.majorPeak();
        lastFrequencyTime = now;
    }

    // Lautstärke-Timer aktualisieren
    if (now - lastLoudnessTime >= LOUDNESS_INTERVAL) {
        lastLoudnessTime = now;
    }
}

bool INMP441Mic::isNewLoudnessReady() const {
    return (millis() - lastLoudnessTime >= LOUDNESS_INTERVAL);
}

bool INMP441Mic::isNewFrequencyReady() const {
    return (millis() - lastFrequencyTime >= FREQUENCY_INTERVAL);
}

float INMP441Mic::getAverage_dB() const {
    float avgRMS = (volumeCount > 0) ? (sumVolume / volumeCount) : 0.0;
    return convertTo_dB(avgRMS);
}

float INMP441Mic::getDominantFrequency() const {
    return lastDominantFreq;
}

float INMP441Mic::calculateRMS(const int32_t* samples, size_t count) {
    if (count == 0) return 0.0f;
    float sum = 0.0f;
    for (size_t i = 0; i < count; i++) {
        int16_t sample = (int16_t)samples[i];
        sum += (float)sample * sample;
    }
    return sqrt(sum / count);
}

float INMP441Mic::convertTo_dB(float rms) const {
    return 20 * log10(rms / 1.2 + 1.0);
}