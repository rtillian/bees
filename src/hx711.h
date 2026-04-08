// =============================================
// INMP441 Microphone – Hardware / Simulation Wrapper
// =============================================

#ifdef SIMULATION
    #include "hx711_sim.h"
    using HX711_Scale = HX711_Scale_sim;      // Simulations-Version
    #define MIC_IS_SIMULATION 1
#else
    #include "hx711_real.h"
    using HX711_Scale = HX711_Scale;          // Echte Hardware-Version
    #define MIC_IS_SIMULATION 0
#endif