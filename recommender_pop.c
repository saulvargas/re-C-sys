#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

double* recommender_pop_recommend(int uid, recdata_t* recdata, void* args) {
    int iid;
    double* scores;

    (void) uid;
    (void) args;

    scores = calloc(recdata->N_items, sizeof(double));
    for (iid = 0; iid < recdata->N_items; iid++) {
        scores[iid] = recdata_itemdata_size(recdata, iid);
    }

    return scores;
}

void recommender_pop_close(recdata_t* recdata, void* args) {
    (void) recdata;
    (void) args;
}

recommender_t* recommender_pop_create(recdata_t* recdata) {
    recommender_t* recommender;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_pop_recommend;
    recommender->close = recommender_pop_close;
    recommender->recdata = recdata;
    recommender->args = NULL;
    
    return recommender;
}
