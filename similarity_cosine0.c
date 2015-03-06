#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "similarity.h"
#include "topn.h"

idpairs_t* similarity_cosine0_calculate(int uid, int k, void* args) {
    recdata_t* recdata;
    int* coo;
    idpairs_t* ud;
    idpairs_t* id;
    int i;
    int j;
    int vid;
    int iid;
    topn_t* topn;
    idpairs_t* pairs;
    
    recdata = args;
    
    coo = calloc(recdata->N_users, sizeof(int));
    
    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        iid = idpairs_keys(ud)[i];
        id = recdata_userdata(recdata, iid);
        for (j = 0; j < idpairs_size(id); j++) {
            vid = idpairs_keys(id)[j];
            coo[vid]++;
        }
        idpairs_close_shallow(id);
    }
    idpairs_close_shallow(ud);
    
    coo[uid] = 0;
    
    topn = topn_create(k);
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            topn_add(topn, vid, coo[vid] / sqrt(recdata_userdata_size(recdata, uid) * recdata_userdata_size(recdata, vid)));
        }
    }
    
    free(coo);

    pairs = topn_get_pairs(topn);
    
    topn_close(topn);

    return pairs;
}

void similarity_cosine0_close(void* args) {
    (void) args;
    return;
}

similarity_t* similarity_cosine0_create(recdata_t* recdata) {
    similarity_t* similarity;
    
    similarity = malloc(sizeof(similarity_t));
    similarity->calculate = similarity_cosine0_calculate;
    similarity->close = similarity_cosine0_close;
    similarity->args = recdata;
    
    return similarity;
}
