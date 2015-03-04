#include "similarity.h"

sim_t* similarity_calculate(similarity_t* sim, int uid) {
    return sim->calculate(uid, sim->args);
}

void similarity_close(similarity_t* sim) {
    sim->close(sim->args);
    
    free(sim);
}

