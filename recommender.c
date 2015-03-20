#include <stdlib.h>
#include "idpairs.h"
#include "recommender.h"
#include "topn.h"

idpairs_t* recommender_recommend(recommender_t* recommender, int uid, int n) {
    recdata_t* recdata;
    idpairs_t* pairs;
    idpairs_t* ud;
    int i;
    int iid;
    int jid;
    topn_t* topn;
    double* scores;
    
    recdata = recommender->recdata;
    
    scores = recommender->recommend(uid, recommender->recdata, recommender->args);

    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        jid = idpairs_keys(ud)[i];
        scores[jid] = 0.0;
    }
    idpairs_close_shallow(ud);

    topn = topn_create(n);
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

void recommender_close(recommender_t* recommender) {
    recommender->close(recommender->recdata, recommender->args);
    
    free(recommender);
}

