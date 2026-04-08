// hx711_real.cpp
#include "hx711_real.h"

HX711_Scale::HX711_Scale() {
    // Konstruktor
}

bool HX711_Scale::begin() {
    Serial.println("Initialisiere HX711 Load Cell...");

    scale.begin(DT_PIN, SCK_PIN);
    
    // Warte kurz bis der HX711 bereit ist
    if (scale.is_ready()) {
        scale.set_scale(calibration_factor);
        scale.tare();                    // Automatisches Tara beim Start
        initialized = true;
        
        Serial.println("HX711 erfolgreich initialisiert und getared.");
        return true;
    } else {
        Serial.println("Fehler: HX711 nicht gefunden!");
        return false;
    }
}

float HX711_Scale::measure_weight() {
    if (!initialized) {
        Serial.println("HX711 nicht initialisiert!");
        return 0.0;
    }

    if (scale.is_ready()) {
        // 10 Messungen mitteln für stabilere Werte
        float weight = scale.get_units(10);
        return weight;
    } else {
        Serial.println("HX711 nicht bereit...");
        return 0.0;
    }
}

void HX711_Scale::tare() {
    if (initialized) {
        scale.tare();
        Serial.println("Tara durchgeführt (Nullpunkt gesetzt).");
    }
}

void HX711_Scale::setCalibrationFactor(float factor) {
    calibration_factor = factor;
    if (initialized) {
        scale.set_scale(calibration_factor);
    }
    Serial.printf("Kalibrierungsfaktor gesetzt: %.2f\n", factor);
}

float HX711_Scale::getCalibrationFactor() const {
    return calibration_factor;
}