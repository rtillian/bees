#include "supabase_logic.h"

std::string buildSupabaseJson(const BienenDaten& d) {
    std::string json = "{";
    json += "\"boxnumber\":" + std::to_string(d.boxnumber) + ",";
    json += "\"temperature\":" + std::to_string(d.temperature);
    json += "}";
    return json;
}

std::string buildSupabaseUrl(const std::string& baseUrl, const std::string& table) {
    return baseUrl + "/rest/v1/" + table;  
}
