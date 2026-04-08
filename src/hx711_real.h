// hx711_real.h
#pragma once

#include <Arduino.h>
#include <HX711.h>         

class HX711_Scale {
private:
    HX711_Scale scale;
    
    const int DT_PIN  = 16;
    const int SCK_PIN = 17;
    
    float calibration_factor = 1.0;
    bool initialized = false;

public:
    HX711_Scale();                    // Konstruktor
    
    bool begin();                     
    float measure_weight();           
    
    void tare();                      
    void setCalibrationFactor(float factor);
    float getCalibrationFactor() const;
};