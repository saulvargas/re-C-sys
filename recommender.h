#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "aux.h"
#include "recdata.h"
#include "similarity.h"

typedef struct recommender_ {
    idpairs_t* (* recommend)(int uid, int n, void* args);
    void (* close)(void* args);
    void* args;
} recommender_t;

idpairs_t* recommender_recommend(recommender_t* recommender, int uid, int n);

void recommender_close(recommender_t* recommender);

recommender_t* recommender_ubknn_create(recdata_t* recdata, similarity_t* similarity, int k);

recommender_t* recommender_ibknn_create(recdata_t* recdata, similarity_t* similarity, int k);

#endif
