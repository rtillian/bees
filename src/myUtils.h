#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <Arduino.h>
#include <WiFi.h>

class MyUtils {
public:
    MyUtils();                    // Konstruktor
    String getDeviceID();         // Gibt die MAC-Adresse als String zurück (ohne Doppelpunkte)
    void   printDeviceInfo();     // Gibt Geräte-Infos auf Serial aus

private:
    String deviceID = "";
};

#endif