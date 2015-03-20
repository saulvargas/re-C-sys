#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "idpairs.h"
#include "recdata.h"
#include "similarity.h"

typedef struct recommender_ {
    double* (* recommend)(int uid, recdata_t* recdata, void* args);
    void (* close)(recdata_t* recdata, void* args);
    recdata_t* recdata;
    void* args;
} recommender_t;

idpairs_t* recommender_recommend(recommender_t* recommender, int uid, int n);

void recommender_close(recommender_t* recommender);

recommender_t* recommender_ub_create(recdata_t* recdata, similarity_t* similarity, int k, int binary);

recommender_t* recommender_ib_create(recdata_t* recdata, similarity_t* similarity, int k, int binary);

recommender_t* recommender_pop_create(recdata_t* recdata);

#endif
