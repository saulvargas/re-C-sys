#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "recdata.h"

recdata_t* recdata_transposed_create(recdata_t* recdata0) {
    recdata_t* recdata;
    
    recdata = malloc(sizeof(recdata_t));
    recdata->N_users = recdata0->N_items;
    recdata->N_items = recdata0->N_users;
    recdata->N_prefs = recdata0->N_prefs;
    
    recdata->userdata_size = recdata0->itemdata_size;
    recdata->itemdata_size = recdata0->userdata_size;
    recdata->userdata = recdata0->itemdata;
    recdata->itemdata = recdata0->userdata;
    recdata->close = recdata0->close;
    
    recdata->args = NULL;
    
    return recdata;    
}

