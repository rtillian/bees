// =============================================
// INMP441 Microphone – Hardware / Simulation Wrapper
// =============================================

#ifdef SIMULATION
    #include "inmp441_mic_sim.h"
    using INMP441Mic = INMP441Mic_sim;      // Simulations-Version
    #define MIC_IS_SIMULATION 1
#else
    #include "inmp441_mic_real.h"
    using INMP441Mic = INMP441Mic;          // Echte Hardware-Version
    #define MIC_IS_SIMULATION 0
#endif