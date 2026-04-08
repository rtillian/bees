#include "data_buffer.h"
#include <ArduinoJson.h>

DataBuffer::DataBuffer() {}

void DataBuffer::addData(
        String beeStation, 
        long boxNumber, 
        float tempAHT, 
        float tempBMP,
        float humidity, 
        float airPressure,
        float seaLevel,
        float volume,
        long frequency,
        float co2, 
        unsigned long beeCountIn, 
        unsigned long beeCountOut 
) 
{
    
    if (recordCount >= MAX_RECORDS) {
        // Ringbuffer: älteste Einträge überschreiben
        for (int i = 0; i < MAX_RECORDS - 1; i++) {
            records[i] = records[i + 1];
        }
        recordCount = MAX_RECORDS - 1;
    }

    records[recordCount].tempAHT = tempAHT;
    records[recordCount].tempBMP = tempBMP;
    records[recordCount].humidity = humidity;
    records[recordCount].airPressure = airPressure;
    records[recordCount].seaLevel = seaLevel;
    records[recordCount].volume = volume;
    records[recordCount].frequency = frequency;
    records[recordCount].co2 = co2;
    records[recordCount].beeCountIn = beeCountIn;
    records[recordCount].beeCountOut = beeCountOut;
    //records[recordCount].timestamp = millis();

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
    String beeStation = "abc";
    unsigned long beeCountIn;
    unsigned long beeCountOut;
    for (int i = 0; i < recordCount; i++) {
        if (!supabase.sendData(
 
                records[i].beeStation,
                records[i].boxNumber,
                records[i].tempAHT,
                records[i].tempBMP,
                records[i].humidity,
                records[i].airPressure,
                records[i].seaLevel,
                records[i].volume,
                records[i].frequency,
                records[i].co2,
                records[i].beeCountIn,
                records[i].beeCountOut
            )
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
    Serial.printf("Responce - httpCode: %d: ", httpCode);

    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Station-Info gained: " + payload);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error && doc.is<JsonArray>() && doc.size() > 0) {
            JsonObject row = doc[0];

            beeStation = row["bee_location"] | "";
            boxNumber  = row["box_number"] | 0;

            Serial.printf("→ Bee Location: %s\r\n", beeStation.c_str());
            Serial.printf("→ Box Number : %d\r\n", boxNumber);

            http.end();
            return true;
        } 
        else {
            Serial.println("JSON-Parsing went wrong - no data found");
        }
    } 
    else if (httpCode == 404 || httpCode == 406) {
        Serial.println("MAC-Address in table locations not found.");
    } else {
        Serial.println("Error");
        String errorPayload = http.getString();
        Serial.printf("httpCode %d: ", httpCode);
        Serial.println(errorPayload);
      
    }
    return false;
}


