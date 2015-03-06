#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

typedef struct {
    recdata_t* recdata;
    similarity_t* similarity;
    int k;
} recommender_ubknn_args_t;

idpairs_t* recommender_ubknn_recommend(int uid, int N, void* args) {
    idpairs_t* neighbors;
    idpairs_t* pairs;
    idpairs_t* vd;
    int i;
    int j;
    int vid;
    double s;
    int iid;
    topn_t* topn;
    recdata_t* recdata = ((recommender_ubknn_args_t*) args)->recdata;
    similarity_t* similarity = ((recommender_ubknn_args_t*) args)->similarity;
    int k = ((recommender_ubknn_args_t*) args)->k;
    
    double* scores = calloc(recdata->N_items, sizeof(double));
    
    neighbors = similarity_calculate(similarity, uid, k);
    for (i = 0; i < idpairs_size(neighbors); i++) {
        vid = idpairs_keys(neighbors)[i];
        vd = recdata_userdata(recdata, vid);
        s = idpairs_values(neighbors)[i];
        for (j = 0; j < idpairs_size(vd); j++) {
            iid = idpairs_keys(vd)[j];
            scores[iid] += s * idpairs_values(vd)[j];
        }
        idpairs_close_shallow(vd);
    }
    
    idpairs_close_deep(neighbors);
    
    topn = topn_create(N);
    for (iid = 0; iid < recdata->N_items; iid++) {
        if (scores[iid] > 0) {
            topn_add(topn, iid, scores[iid]);
        }
    }
    
    free(scores);

    topn_sort(topn);
    pairs = topn_get_pairs(topn);
    idpairs_revert(pairs);
    
    topn_close(topn);

    return pairs;

}

void recommender_ubknn_close(void* args) {
    free(args);
}

recommender_t* recommender_ubknn_create(recdata_t* recdata, similarity_t* similarity, int k) {
    recommender_t* recommender;
    recommender_ubknn_args_t* args;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_ubknn_recommend;
    recommender->close = recommender_ubknn_close;
    
    args = malloc(sizeof(recommender_ubknn_args_t));
    args->recdata = recdata;
    args->similarity = similarity;
    args->k = k;
    recommender->args = args;
    
    return recommender;
}
