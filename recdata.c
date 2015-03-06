#include "recdata.h"

int recdata_userdata_size(recdata_t* recdata, int uid) {
    return recdata->userdata_size(uid, recdata->args);
}

int recdata_itemdata_size(recdata_t* recdata, int iid) {
    return recdata->itemdata_size(iid, recdata->args);
}

idpairs_t* recdata_userdata(recdata_t* recdata, int uid) {
    return recdata->userdata(uid, recdata->args);
}

idpairs_t* recdata_itemdata(recdata_t* recdata, int iid) {
    return recdata->itemdata(iid, recdata->args);
}

void recdata_close(recdata_t* recdata) {
    recdata->close(recdata->args);
    
    free(recdata);
}
