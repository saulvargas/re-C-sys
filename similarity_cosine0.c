#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "similarity.h"
#include "topn.h"

idpairs_t* similarity_cosine0_calculate(int uid, int k, void* args) {
    recdata_t* recdata;
    int* coo;
    int n1;
    int n2;
    int* v1;
    int* v2;
    int i;
    int j;
    int vid;
    int iid;
    topn_t* topn;
    idpairs_t* pairs;
    
    recdata = args;
    
    coo = calloc(recdata->N_users, sizeof(int));
    
    n1 = recdata_n_items(recdata, uid);
    v1 = recdata_v_items(recdata, uid);
    for (i = 0; i < n1; i++) {
        iid = v1[i];
        n2 = recdata_n_users(recdata, iid);
        v2 = recdata_v_users(recdata, iid);
        for (j = 0; j < n2; j++) {
            vid = v2[j];
            coo[vid]++;
        }
    }
    
    coo[uid] = 0;
    
    topn = topn_create(k);
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            topn_add(topn, vid, coo[vid] / sqrt(n1 * recdata_n_items(recdata, vid)));
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
