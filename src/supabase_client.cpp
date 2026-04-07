#include "supabase_client.h"

SupabaseClient::SupabaseClient() {}

bool SupabaseClient::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);

#ifdef SIMULATION
    // ====================== WOKWI SIMULATION ======================
    Serial.println("→ Simulation: Verbinde mit Wokwi-GUEST");
    WiFi.begin("Wokwi-GUEST", "", 6);        // Channel 6 = schneller
#else
    // ====================== ECHTER ESP32 ======================
    Serial.print("→ Hardware: Verbinde mit WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
#endif

    Serial.print("Warte auf Verbindung");

    int attempts = 0;
    const int maxAttempts = 40;                 // mehr Versuche in Simulation sinnvoll

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\r\n✓ WiFi verbunden!");
        Serial.print("   IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("   SSID: ");
        Serial.println(WiFi.SSID());

        deviceID = WiFi.macAddress();
        Serial.printf("   Geräte-ID: %s\r\n", deviceID.c_str());

        return true;
    } 

    Serial.println("\r\n✗ WiFi-Verbindung fehlgeschlagen!");
    Serial.printf("   Letzter Status: %d\r\n", WiFi.status());
    return false;
}
// ... (sendData und andere Funktionen bleiben gleich wie vorher)

bool SupabaseClient::sendData(
        String beeStation, 
        long boxNumber, 
        float tempAHT, 
        float tempBMP, 
        float humidity, 
        float airPressure,
        float seaLevel,
        float volume,
        long  frequency,
        float co2, 
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
    doc["temp_aht20"]     = tempAHT;
    doc["temp_bmp280"]    = tempBMP;
    doc["humidity"]       = humidity;
    doc["air_pressure"]   = airPressure;
    doc["sea_level"]      = seaLevel;
    doc["volume"]         = volume;
    doc["frequency"]      = frequency;
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
        Serial.printf("Erfolg! Code: %d\r\n", httpResponseCode);
        if (response.length() > 0) Serial.println("Antwort: " + response);
        http.end();
        return true;
    } 
    else {
        Serial.printf("Fehler beim Senden: %d\r\n", httpResponseCode);
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