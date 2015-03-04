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
    
    recdata->n_items = recdata0->n_users;
    recdata->v_items = recdata0->v_users;
    recdata->n_users = recdata0->n_items;
    recdata->v_users = recdata0->v_items;
    recdata->close = recdata0->close;
    
    recdata->args = NULL;
    
    return recdata;    
}

