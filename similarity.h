#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "idpairs.h"

typedef struct similarity_ {
    idpairs_t* (* calculate)(int uid, int k, void* args);
    void (* close)(void* args);
    void* args;
} similarity_t;

idpairs_t* similarity_calculate(similarity_t* sim, int uid, int k);

void similarity_close(similarity_t* sim);

similarity_t* similarity_setcosine_create(recdata_t* recdata);

similarity_t* similarity_veccosine_create(recdata_t* recdata);

#endif
