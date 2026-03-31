#ifndef SUPABASE_CLIENT_H
#define SUPABASE_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class SupabaseClient {
public:
    SupabaseClient();
    bool begin(const char* ssid, const char* password);
    bool sendData(float dB, float frequency, long beeCount);
    bool isConnected() const;

private:
    bool connectWiFi();

    const char* supabaseURL = "https://gxboqujtughycebpejxp.supabase.co";
    const char* anonKey     = "sbp_v0_fb0e289fad759e188f5e23234ed5463588ef21b7";
    
    String deviceID;
    unsigned long lastSendTime = 0;
    const unsigned long SEND_INTERVAL = 30000;  // 30 Sekunden
};

#endif