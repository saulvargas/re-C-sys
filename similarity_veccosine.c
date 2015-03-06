#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "similarity.h"
#include "topn.h"

typedef struct {
    recdata_t* recdata;
    double* norm;
} similarity_veccosine_args_t;

idpairs_t* similarity_veccosine_calculate(int uid, int k, void* args) {
    recdata_t* recdata;
    double* norm;
    double* coo;
    idpairs_t* ud;
    idpairs_t* id;
    int i;
    int j;
    int vid;
    int iid;
    double r;
    topn_t* topn;
    idpairs_t* pairs;
    
    recdata = ((similarity_veccosine_args_t*) args)->recdata;
    norm = ((similarity_veccosine_args_t*) args)->norm;
    
    coo = calloc(recdata->N_users, sizeof(double));
    
    ud = recdata_userdata(recdata, uid);
    for (i = 0; i < idpairs_size(ud); i++) {
        iid = idpairs_keys(ud)[i];
        r = idpairs_values(ud)[i];
        id = recdata_itemdata(recdata, iid);
        for (j = 0; j < idpairs_size(id); j++) {
            vid = idpairs_keys(id)[j];
            coo[vid] += r * idpairs_values(id)[j];
        }
        idpairs_close_shallow(id);
    }
    idpairs_close_shallow(ud);
    
    coo[uid] = 0.0;
    
    topn = topn_create(k);
    for (vid = 0; vid < recdata->N_users; vid++) {
        if (coo[vid] > 0) {
            topn_add(topn, vid, coo[vid] * norm[uid] * norm[vid]);
        }
    }
    
    free(coo);

    pairs = topn_get_pairs(topn);
    
    topn_close(topn);

    return pairs;
}

void similarity_veccosine_close(void* args) {
    free(((similarity_veccosine_args_t*) args)->norm);
    free(args);
}

double* calculate_norm(recdata_t* recdata) {
    int i;
    int uid;
    idpairs_t* ud;
    double* norm;
    
    norm = malloc(recdata->N_users * sizeof(double));
     
    for (uid = 0; uid < recdata->N_users; uid++) {
        ud = recdata_userdata(recdata, uid);
        norm[uid] = 0.0;
        for (i = 0; i < idpairs_size(ud); i++) {
            norm[uid] += idpairs_values(ud)[i] * idpairs_values(ud)[i];
        }
        norm[uid] = 1 / sqrt(norm[uid]);
        idpairs_close_shallow(ud);
    }
     
    return norm;
}

similarity_t* similarity_veccosine_create(recdata_t* recdata) {
    similarity_t* similarity;
    similarity_veccosine_args_t* args;
    
    similarity = malloc(sizeof(similarity_t));
    similarity->calculate = similarity_veccosine_calculate;
    similarity->close = similarity_veccosine_close;
    
    args = malloc(sizeof(similarity_veccosine_args_t));
    args->recdata = recdata;
    args->norm = calculate_norm(recdata);
    similarity->args = args;
    
    return similarity;
}
