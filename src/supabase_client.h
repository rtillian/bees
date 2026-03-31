#ifndef SUPABASE_CLIENT_H
#define SUPABASE_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class SupabaseClient {
public:
    SupabaseClient();
    bool begin(const char* ssid, const char* password);
    bool sendData(
        String beeStation, 
        long boxNumber, 
        float temperature, 
        float humidity, 
        float co2, 
        float db, 
        long frequency,
        unsigned long beeCountIn, 
        unsigned long beeCountOut 
    );

    bool isConnected() const;

    // Öffentlich zugänglich für DataBuffer
    const char* supabaseURL = "https://gxboqujtughycebpejxp.supabase.co";
    const char* anonKey     = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imd4Ym9xdWp0dWdoeWNlYnBlanhwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzQ3OTMwNzksImV4cCI6MjA5MDM2OTA3OX0.amyYYqykRBuJ8I45aL7qPeQ9tHOnj7wiKkyLWB_i2uo";
    String deviceID;

private:
    bool connectWiFi();

    unsigned long lastSendTime = 0;
    const unsigned long SEND_INTERVAL = 30000;  // 30 Sekunden
};

#endif