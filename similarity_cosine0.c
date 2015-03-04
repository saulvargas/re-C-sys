#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "similarity.h"
#include "topn.h"

sim_t* similarity_cosine0_calculate(int uid, int N, void* args) {
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
    sim_t* sim;
    
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
    
    topn = topn_create(N);
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            topn_add(topn, vid, coo[vid] / sqrt(n1 * recdata_n_items(recdata, vid)));
        }
    }
    
    free(coo);
    
    sim = malloc(sizeof(sim_t));
    sim->n = topn_size(topn);
    sim->uids = malloc(sim->n * sizeof(int));
    memcpy(sim->uids, topn_get_keys(topn), sim->n * sizeof(int));
    sim->sims = malloc(sim->n * sizeof(double));
    memcpy(sim->sims, topn_get_values(topn), sim->n * sizeof(double));
    
    topn_close(topn);

    return sim;
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
