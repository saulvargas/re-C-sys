#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

idpairs_t* recommender_pop_recommend(int uid, int N, void* args) {
    int i;
    int* keys = malloc(N * sizeof(int));
    double* values = malloc(N * sizeof(double));
    idpairs_t* pop = args;
    
    (void) uid;
    
    for (i = 0; i < N; i++) {
        keys[i] = idpairs_keys(pop)[i];
        values[i] = idpairs_values(pop)[i];
    }

    return idpairs_create(N, keys, values);
}

void recommender_pop_close(void* args) {
    idpairs_close_deep(args);
}

recommender_t* recommender_pop_create(recdata_t* recdata) {
    topn_t* topn;
    int iid;
    recommender_t* recommender;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_pop_recommend;
    recommender->close = recommender_pop_close;

    topn = topn_create(recdata->N_items);
    for (iid = 0; iid < recdata->N_items; iid++) {
        topn_add(topn, iid, recdata_itemdata_size(recdata, iid));
    }
    
    topn_sort(topn);
    recommender->args = topn_get_pairs(topn);
    idpairs_revert(recommender->args);
    topn_close(topn);
    
    return recommender;
}
