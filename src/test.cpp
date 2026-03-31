#include <iostream>
#include "supabase_logic.h"

int main() {
    BienenDaten d{5, 34.2f};

    std::cout << "JSON: " << buildSupabaseJson(d) << "\n";
    std::cout << "URL:  " << buildSupabaseUrl("https://xyz.supabase.co", "Bienen") << "\n";
}

