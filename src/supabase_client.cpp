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

        deviceID = WiFi.macAddress();
        //deviceID.replace(":", "");
        Serial.printf("Geräte-ID: %s\n", deviceID.c_str());
        return true;
    } 

    Serial.println("\nWiFi-Verbindung fehlgeschlagen!");
    return false;
}

// ... (sendData und andere Funktionen bleiben gleich wie vorher)

bool SupabaseClient::sendData(
        String beeStation, 
        long boxNumber, 
        float temperature, 
        float humidity, 
        float co2, 
        float db, 
        long frequency,
        unsigned long beeCountIn, 
        unsigned long beeCountOut 
) {
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Keine WiFi-Verbindung. Versuche Reconnect...");
        if (!connectWiFi()) return false;
    }

    Serial.println("Sende Daten an Supabase (Tabelle: sensor_data)...");

    // ArduinoJson Dokument
    JsonDocument doc;
    doc["bee_station"]    = beeStation;        // aus DataBuffer
    doc["box_number"]     = boxNumber;         // aus DataBuffer
    doc["temperature"]    = temperature;
    doc["humidity"]       = humidity;
    doc["co2"]            = co2;
    doc["bee_count_in"]   = beeCountIn;
    doc["bee_count_out"]  = beeCountOut;

    // JSON in String umwandeln
    String payload;
    serializeJson(doc, payload);

    HTTPClient http;
    String url = String(supabaseURL) + "/rest/v1/sensor_data";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", anonKey);
    http.addHeader("Authorization", "Bearer " + String(anonKey));

    Serial.print("Payload: ");
    Serial.println(payload);

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("Erfolg! Code: %d\n", httpResponseCode);
        if (response.length() > 0) Serial.println("Antwort: " + response);
        http.end();
        return true;
    } 
    else {
        Serial.printf("Fehler beim Senden: %d\n", httpResponseCode);
        String errorResponse = http.getString();
        if (errorResponse.length() > 0) {
            Serial.println("Supabase Fehlermeldung: " + errorResponse);
        }
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