#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <Arduino.h>
#include "supabase_client.h"

struct SensorRecord {
    String beeStation;
    long boxNumber;
    float temperature; 
    float humidity;
    float co2;
    float db;
    long frequency;
    unsigned long beeCountIn; 
    unsigned long beeCountOut; 
    unsigned long timestamp;
};

class DataBuffer {
public:
    DataBuffer();
    void addData(
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
    bool uploadBuffer(SupabaseClient& supabase);

    String getBeeStation() const;
    String getBoxNumber() const;
    String beeStation = "";
    String boxNumber = "";


private:
    static constexpr int MAX_RECORDS = 60;
    SensorRecord records[MAX_RECORDS];
    int recordCount = 0;

 
    bool stationInfoLoaded = false;

    bool checkAndLoadStationInfo(SupabaseClient& supabase);   // echte Abfrage
};

#endif