#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "idpairs.h"
#include "recdata.h"
#include "similarity.h"

typedef struct recommender_ {
    idpairs_t* (* recommend)(int uid, int n, void* args);
    void (* close)(void* args);
    void* args;
} recommender_t;

idpairs_t* recommender_recommend(recommender_t* recommender, int uid, int n);

void recommender_close(recommender_t* recommender);

recommender_t* recommender_ub_create(recdata_t* recdata, similarity_t* similarity, int k, int binary);

recommender_t* recommender_ib_create(recdata_t* recdata, similarity_t* similarity, int k, int binary);

#endif
