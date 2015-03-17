#include <stdlib.h>
#include "recommender.h"
#include "topn.h"

idpairs_t* recommender_pop_recommend(int uid, int N, void* args) {
    idpairs_t* pairs;
    idpairs_t* ud;
    int i;
    int iid;
    int jid;
    topn_t* topn;
    recdata_t* recdata = args;
    
    double* scores = calloc(recdata->N_items, sizeof(double));
    
    for (iid = 0; iid < recdata->N_items; iid++) {
        scores[iid] = recdata_itemdata_size(recdata, iid);
    }

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

void recommender_pop_close(void* args) {
    (void) args;
}

recommender_t* recommender_pop_create(recdata_t* recdata) {
    recommender_t* recommender;
    
    recommender = malloc(sizeof(recommender_t));
    recommender->recommend = recommender_pop_recommend;
    recommender->close = recommender_pop_close;
    recommender->args = recdata;
    
    return recommender;
}
