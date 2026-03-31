#include "supabase_client.h"

SupabaseClient::SupabaseClient() {}

bool SupabaseClient::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Verbinde mit WiFi: ");
    Serial.println(ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 25) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi verbunden!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        // Device ID holen
        deviceID = WiFi.macAddress();
        deviceID.replace(":", "");
        Serial.printf("Geräte-ID: %s\n", deviceID.c_str());
        return true;
    } 

    Serial.println("\nWiFi-Verbindung fehlgeschlagen!");
    return false;
}

// ... (sendData und andere Funktionen bleiben gleich wie vorher)

bool SupabaseClient::sendData(float dB, float frequency, long beeCount) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Keine WiFi-Verbindung. Versuche Reconnect...");
        connectWiFi();
        if (WiFi.status() != WL_CONNECTED) return false;
    }

    HTTPClient http;
    String url = String(supabaseURL) + "/rest/v1/locations";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", anonKey);
    http.addHeader("Authorization", "Bearer " + String(anonKey));

    // JSON Payload
    String payload = "{";
    payload += "\"mac_address\":\"" + deviceID + "\",";
    payload += "\"decibel\":" + String(dB, 1) + ",";
    payload += "\"frequency\":" + String(frequency, 0) + ",";
    payload += "\"bee_count\":" + String(beeCount);
    payload += "}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
        Serial.printf("Daten gesendet → Code: %d\n", httpResponseCode);
        http.end();
        return true;
    } else {
        Serial.printf("Fehler beim Senden: %d\n", httpResponseCode);
        http.end();
        return false;
    }
}

bool SupabaseClient::connectWiFi() {
    if (WiFi.status() == WL_CONNECTED) return true;
    
    WiFi.reconnect();
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(500);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}

bool SupabaseClient::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}