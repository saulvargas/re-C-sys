#ifndef RECDATA_H
#define RECDATA_H

#include <stdlib.h>
#include <stdio.h>

struct recdata_ {
    int N_users;
    int N_items;
    int N_prefs;
    int (* n_items)(int uid, void* args);
    int* (* v_items)(int uid, void* args);
    int (* n_users)(int iid, void* args);
    int* (* v_users)(int uidx, void* args);
    void (* close)(void* args);
    void* args;
};

typedef struct recdata_ recdata_t;

int recdata_n_items(recdata_t* recdata, int uid);

int* recdata_v_items(recdata_t* recdata, int uid);

int recdata_n_users(recdata_t* recdata, int iid);

int* recdata_v_users(recdata_t* recdata, int iid);

void recdata_close(recdata_t* recdata);

recdata_t* recdata_simple_create(FILE* user_data, FILE* item_data, int N_users, int N_items, int N_prefs);

#endif
