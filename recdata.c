#include "recdata.h"

int recdata_n_items(recdata_t* recdata, int uid) {
    return recdata->n_items(uid, recdata->args);
}

int* recdata_v_items(recdata_t* recdata, int uid) {
    return recdata->v_items(uid, recdata->args);
}

int recdata_n_users(recdata_t* recdata, int iid) {
    return recdata->n_users(iid, recdata->args);
}

int* recdata_v_users(recdata_t* recdata, int iid) {
    return recdata->v_users(iid, recdata->args);
}

void recdata_close(recdata_t* recdata) {
    recdata->close(recdata->args);
    
    free(recdata);
}
