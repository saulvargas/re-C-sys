#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"

typedef struct {
    int n;
    int* uids;
    double* sims;
} sim_t;

struct similarity_ {
    sim_t* (* calculate)(int uid, void* args);
    void (* close)(void* args);
    void* args;
};

typedef struct similarity_ similarity_t;

sim_t* similarity_calculate(similarity_t* sim, int uid);

void similarity_close(similarity_t* sim);

similarity_t* similarity_cosine0_create(recdata_t* recdata);

#endif
