#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

typedef struct {
    recdata_t* recdata;
    idpairs_t** cached_neighbors;
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
    idpairs_t** cached_neighbors = ((recommender_ib_args_t*) args)->cached_neighbors;
    int binary = ((recommender_ib_args_t*) args)->binary;
    
    double* scores = calloc(recdata->N_items, sizeof(double));
    
    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        jid = idpairs_keys(ud)[i];
        neighbors = cached_neighbors[jid];
        s = binary ? 1.0 : idpairs_values(ud)[i];
        for (j = 0; j < idpairs_size(neighbors); j++) {
            iid = idpairs_keys(neighbors)[j];
            scores[iid] += s * idpairs_values(neighbors)[j];
        }
    }
    idpairs_close_shallow(ud);

    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        jid = idpairs_keys(ud)[i];
        scores[jid] = 0.0;
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
    int iid;
    int N_items;
    recommender_ib_args_t* a = args;
    
    N_items = a->recdata->N_items;
    for (iid = 0; iid < N_items; iid++) {
        idpairs_close_deep(a->cached_neighbors[iid]);
    }
    free(a->cached_neighbors);

    free(args);
}

idpairs_t** calculate_neighbors(int N_items, similarity_t* similarity, int k) {
    int iid;
    idpairs_t** idpairs;
    
    idpairs = malloc(N_items * sizeof(idpairs_t*));
    for (iid = 0; iid < N_items; iid++) {
        idpairs[iid] = similarity_calculate(similarity, iid, k);
    }
    
    return idpairs;
}

recommender_t* recommender_ib_create(recdata_t* recdata, similarity_t* similarity, int k, int binary) {
    recommender_t* recommender;
    recommender_ib_args_t* args;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_ib_recommend;
    recommender->close = recommender_ib_close;
    
    args = malloc(sizeof(recommender_ib_args_t));
    args->recdata = recdata;
    args->cached_neighbors = calculate_neighbors(recdata->N_items, similarity, k);
    args->binary = binary;
    recommender->args = args;
    
    return recommender;
}
