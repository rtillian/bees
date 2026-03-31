#include "data_buffer.h"
#include <ArduinoJson.h>

DataBuffer::DataBuffer() {}

void DataBuffer::addData(
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
    
    if (recordCount >= MAX_RECORDS) {
        // Ringbuffer: älteste Einträge überschreiben
        for (int i = 0; i < MAX_RECORDS - 1; i++) {
            records[i] = records[i + 1];
        }
        recordCount = MAX_RECORDS - 1;
    }

    records[recordCount].db = db;
    records[recordCount].frequency = frequency;
    records[recordCount].beeCountIn = beeCountIn;
    records[recordCount].beeCountOut = beeCountOut;
    records[recordCount].temperature = temperature;
    records[recordCount].humidity = humidity;
    records[recordCount].co2 = co2;
    records[recordCount].timestamp = millis();

    recordCount++;
}

bool DataBuffer::uploadBuffer(SupabaseClient& supabase) {
    if (recordCount == 0) {
        Serial.println("Puffer ist leer - nichts zu senden.");
        return true;
    }

    // Einmalig Stations-Info laden (bee_station und box_number)
    if (!stationInfoLoaded) {
        stationInfoLoaded = checkAndLoadStationInfo(supabase);
    }

    Serial.printf("Sende %d Datensätze an Supabase (sensor_data)...\n", recordCount);

    bool success = true;

    for (int i = 0; i < recordCount; i++) {
        if (!supabase.sendData(
            
                records[i].dB,
                records[i].frequency,
                records[i].beeCountIn,
                records[i].beeCountOut,
                records[i].temperature,
                records[i].humidity,
                records[i].co2,
                beeStation,      // aus der Station-Info
                boxNumber)
            ) {    // aus der Station-Info
            success = false;
        }
    }

    if (success) {
        Serial.println("Alle Daten erfolgreich an Supabase gesendet.");
        recordCount = 0;        // Puffer leeren
    } else {
        Serial.println("Nicht alle Daten konnten gesendet werden.");
    }

    return success;
}

// ─────────────────────────────────────────────────────────────
// Echte SELECT-Abfrage aus Tabelle "locations"
// ─────────────────────────────────────────────────────────────
bool DataBuffer::checkAndLoadStationInfo(SupabaseClient& supabase) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Keine WiFi-Verbindung für Stations-Abfrage.");
        return false;
    }

    HTTPClient http;
    String url = String(supabase.supabaseURL) + "/rest/v1/locations";
    url += "?select=bee_location,box_number";
    url += "&mac_address=eq." + supabase.deviceID;

    http.begin(url);
    http.addHeader("apikey", supabase.anonKey);
    http.addHeader("Authorization", "Bearer " + String(supabase.anonKey));

    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Station-Info erhalten: " + payload);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error && doc.is<JsonArray>() && doc.size() > 0) {
            JsonObject row = doc[0];

            beeStation = row["bee_location"] | "";
            boxNumber  = row["box_number"] | 0;

            Serial.printf("→ Bee Location: %s\n", beeStation.c_str());
            Serial.printf("→ Box Number : %d\n", boxNumber);

            http.end();
            return true;
        } 
        else {
            Serial.println("JSON-Parsing fehlgeschlagen oder keine Daten gefunden.");
        }
    } 
    else if (httpCode == 404 || httpCode == 406) {
        Serial.println("MAC-Adresse nicht in Tabelle 'locations' gefunden.");
    }
}
