#include <stdlib.h>
#include <math.h>
#include "similarity.h"

sim_t* similarity_cosine0_calculate(int uid, void* args) {
    recdata_t* recdata;
    int* coo;
    int n1;
    int n2;
    int* v1;
    int* v2;
    int i;
    int j;
    int k;
    int vid;
    int iid;
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
    
    sim = malloc(sizeof(sim_t));
    sim->n = 0;
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            sim->n++;
        }
    }
    sim->uids = malloc(sim->n * sizeof(int));
    sim->sims = malloc(sim->n * sizeof(double));
    k = 0;
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            sim->uids[k] = vid;
            sim->sims[k] = coo[vid] / sqrt(n1 * recdata_n_items(recdata, vid));
            k++;
        }
    }
    
    free(coo);
    
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
