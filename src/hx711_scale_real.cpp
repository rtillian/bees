// hx711_real.cpp
#include "hx711_scale_real.h"

HX711_Scale::HX711_Scale() {
    // Konstruktor
}

bool HX711_Scale::begin() {
    Serial.println("=== HX711 Initialisierung ===");
    Serial.print("DT  (GPIO) = "); Serial.println(DT_PIN);
    Serial.print("SCK (GPIO) = "); Serial.println(SCK_PIN);
    
    scale.begin(DT_PIN, SCK_PIN);

    // Warte etwas länger
    delay(100);

    if (scale.is_ready()) {
        Serial.println("HX711 gefunden und bereit!");
        scale.set_scale(calibration_factor);
        scale.tare();
        initialized = true;
        return true;
    } else {
        Serial.println("FEHLER: HX711 nicht gefunden / nicht bereit!");
        Serial.println("Mögliche Ursachen:");
        Serial.println("  - VCC nicht auf 5V (sondern 3.3V)?");
        Serial.println("  - DT oder SCK falsch verdrahtet?");
        Serial.println("  - GND nicht richtig verbunden?");
        Serial.println("  - Load Cell nicht angeschlossen?");
        Serial.println("  - HX711 Modul defekt?");
        initialized = false;
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