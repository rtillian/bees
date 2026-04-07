// =============================================
// INMP441 Microphone – Hardware / Simulation Wrapper
// =============================================

#ifdef SIMULATION
    #include "aht20_bmp280_sim.h"
    using AHT20_BMP280 = AHT20_BMP280_sim;      // Simulations-Version
    #define MIC_IS_SIMULATION 1
#else
    #include "aht20_bmp280_real.h"
    using AHT20_BMP280 = AHT20_BMP280;          // Echte Hardware-Version
    #define MIC_IS_SIMULATION 0
#endif