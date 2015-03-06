#ifndef RECDATA_H
#define RECDATA_H

#include <stdlib.h>
#include <stdio.h>
#include "idpairs.h"

typedef struct recdata_ {
    int N_users;
    int N_items;
    int N_prefs;
    int (* userdata_size)(int uid, void* args);
    int (* itemdata_size)(int iid, void* args);
    idpairs_t* (* userdata)(int uid, void* args);
    idpairs_t* (* itemdata)(int iid, void* args);
    void (* close)(void* args);
    void* args;
} recdata_t;

int recdata_userdata_size(recdata_t* recdata, int uid);

int recdata_itemdata_size(recdata_t* recdata, int iid);

idpairs_t* recdata_userdata(recdata_t* recdata, int uid);

idpairs_t* recdata_itemdata(recdata_t* recdata, int iid);

void recdata_close(recdata_t* recdata);

recdata_t* recdata_simple_create(FILE* user_data, FILE* item_data, int N_users, int N_items, int N_prefs);

#endif
