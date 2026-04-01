#ifdef SIMULATION
    #include "ir_obstacle_sim.h"
    using IRObstacle = IRObstacle_sim;      // Simulations-Version
    #define MIC_IS_SIMULATION 1
#else
    #include "ir_obstacle_real.h"
    using IRObstacle = IRObstacle;      // Simulations-Version
    #define MIC_IS_SIMULATION 0
#endif