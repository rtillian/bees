#pragma once
#include <string>

// Struktur für deine Bienen-Daten
struct BienenDaten {
    int boxnumber;
    float temperature; 
};

// Baut den JSON-String für Supabase
std::string buildSupabaseJson(const BienenDaten& d);

// Baut die vollständige REST-URL für Supabase
std::string buildSupabaseUrl(const std::string& baseUrl, const std::string& table);
