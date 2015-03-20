#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

typedef struct {
    similarity_t* similarity;
    int k;
    int binary;
} recommender_ub_args_t;

double* recommender_ub_recommend(int uid, recdata_t* recdata, void* args) {
    idpairs_t* neighbors;
    idpairs_t* vd;
    int i;
    int j;
    int vid;
    double s;
    int iid;
    similarity_t* similarity = ((recommender_ub_args_t*) args)->similarity;
    int k = ((recommender_ub_args_t*) args)->k;
    int binary = ((recommender_ub_args_t*) args)->binary;
    
    double* scores = calloc(recdata->N_items, sizeof(double));
    
    neighbors = similarity_calculate(similarity, uid, k);
    for (i = 0; i < idpairs_size(neighbors); i++) {
        vid = idpairs_keys(neighbors)[i];
        vd = recdata_userdata(recdata, vid);
        s = idpairs_values(neighbors)[i];
        for (j = 0; j < idpairs_size(vd); j++) {
            iid = idpairs_keys(vd)[j];
            scores[iid] += binary ? (s) : (s * idpairs_values(vd)[j]);
        }
        idpairs_close_shallow(vd);
    }
    idpairs_close_deep(neighbors);

    return scores;
}

void recommender_ub_close(recdata_t* recdata, void* args) {
    (void) recdata;
    free(args);
}

recommender_t* recommender_ub_create(recdata_t* recdata, similarity_t* similarity, int k, int binary) {
    recommender_t* recommender;
    recommender_ub_args_t* args;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_ub_recommend;
    recommender->close = recommender_ub_close;
    recommender->recdata = recdata;
    
    args = malloc(sizeof(recommender_ub_args_t));
    args->similarity = similarity;
    args->k = k;
    args->binary = binary;
    recommender->args = args;
    
    return recommender;
}
