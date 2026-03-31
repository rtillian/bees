#include "inmp441_mic.h"

INMP441Mic::INMP441Mic() {
    // Konstruktor leer
}

void INMP441Mic::begin() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 22050,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 32,      // SCK
        .ws_io_num = 25,       // WS
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = 33      // SD
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 22050, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);

    Serial.println("INMP441 Mikrofon (I2S) initialisiert");
}

float INMP441Mic::calculateRMS(const int32_t* samples, size_t count) {
    float sum = 0.0f;
    for (size_t i = 0; i < count; i++) {
        int32_t sample = samples[i] >> 8;        // 32-Bit → 24-Bit
        sum += sample * sample;
    }
    return sqrt(sum / count);
}

float INMP441Mic::getSoundLevel() {
    size_t bytesRead = 0;
    i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);

    size_t samplesRead = bytesRead / sizeof(int32_t);
    if (samplesRead == 0) return 0.0f;

    return calculateRMS(samples, samplesRead);
}

void INMP441Mic::printSoundLevel() const {
    // Diese Funktion wird nur aufgerufen, wenn vorher getSoundLevel() aufgerufen wurde
    // Für echte Anzeige besser im main loop kombinieren
}