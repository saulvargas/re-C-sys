#include "similarity.h"

idpairs_t* similarity_calculate(similarity_t* sim, int uid, int k) {
    return sim->calculate(uid, k, sim->args);
}

void similarity_close(similarity_t* sim) {
    sim->close(sim->args);
    
    free(sim);
}

