#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

typedef struct {
    recdata_t* recdata;
    similarity_t* similarity;
    int k;
    int binary;
} recommender_ib_args_t;

idpairs_t* recommender_ib_recommend(int uid, int N, void* args) {
    idpairs_t* neighbors;
    idpairs_t* pairs;
    idpairs_t* ud;
    int i;
    int j;
    int jid;
    double s;
    int iid;
    topn_t* topn;
    recdata_t* recdata = ((recommender_ib_args_t*) args)->recdata;
    similarity_t* similarity = ((recommender_ib_args_t*) args)->similarity;
    int k = ((recommender_ib_args_t*) args)->k;
    int binary = ((recommender_ib_args_t*) args)->binary;
    
    double* scores = calloc(recdata->N_items, sizeof(double));
    
    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        jid = idpairs_keys(ud)[i];
        neighbors = similarity_calculate(similarity, jid, k);
        s = binary ? 1.0 : idpairs_values(ud)[i];
        for (j = 0; j < idpairs_size(neighbors); j++) {
            iid = idpairs_keys(neighbors)[j];
            scores[iid] += s * idpairs_values(neighbors)[j];
        }
        idpairs_close_deep(neighbors);
    }
    idpairs_close_shallow(ud);

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

void recommender_ib_close(void* args) {
    free(args);
}

recommender_t* recommender_ib_create(recdata_t* recdata, similarity_t* similarity, int k, int binary) {
    recommender_t* recommender;
    recommender_ib_args_t* args;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_ib_recommend;
    recommender->close = recommender_ib_close;
    
    args = malloc(sizeof(recommender_ib_args_t));
    args->recdata = recdata;
    args->similarity = similarity;
    args->k = k;
    args->binary = binary;
    recommender->args = args;
    
    return recommender;
}
