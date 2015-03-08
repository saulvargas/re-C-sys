#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "recdata.h"

typedef struct {
    int* n_userdata;
    int** i_userdata;
    double** r_userdata;
    int* n_itemdata;
    int** i_itemdata;
    double** r_itemdata;
    int binary;
} recdata_simple_args_t;

int recdata_simple_userdata_size(int uid, void* args) {
    recdata_simple_args_t* a = args;
    return a->n_userdata[uid];
}

int recdata_simple_itemdata_size(int iid, void* args) {
    recdata_simple_args_t* a = args;
    return a->n_itemdata[iid];
}

idpairs_t* recdata_simple_userdata(int uid, void* args) {
    recdata_simple_args_t* a = args;
    return idpairs_create(a->n_userdata[uid], a->i_userdata[uid], a->binary ? NULL : a->r_userdata[uid]);
}

idpairs_t* recdata_simple_itemdata(int iid, void* args) {
    recdata_simple_args_t* a = args;
    return idpairs_create(a->n_itemdata[iid], a->i_itemdata[iid], a->binary ? NULL : a->r_itemdata[iid]);
}

void recdata_simple_close(void* args) {
    recdata_simple_args_t* a = args;
    
    free(a->n_userdata);
    free(a->i_userdata[0]);
    free(a->i_userdata);
    if (!a->binary) {
        free(a->r_userdata[0]);
        free(a->r_userdata);
    }
    free(a->n_itemdata);
    free(a->i_itemdata[0]);
    free(a->i_itemdata);
    if (!a->binary) {
        free(a->r_itemdata[0]);
        free(a->r_itemdata);
    }
    free(a);
}

#define MAX_LEN_LINE 256

void read_data(int** n, int*** v, double*** r, FILE* data, int N, int N_prefs, int binary) {
    char line[MAX_LEN_LINE];
    int last_x;
    int x;
    int y;
    double z;
    int* p;
    double* q;

    *v = malloc(N * sizeof(int*));
    p = malloc(N_prefs * sizeof(int));
    if (!binary) {
        *r = malloc(N * sizeof(double*));
        q = malloc(N_prefs * sizeof(double));
    } else {
        *r = NULL;
        q = NULL;
    }
    last_x = -1;
    while (fgets(line, MAX_LEN_LINE, data) != NULL) {
        x = atoi(strtok(line, "\t"));
        y = atoi(strtok(NULL, "\t"));
        if (!binary) {
            z = atof(strtok(NULL, "\t"));
        }
        
        if (x != last_x) {
            (*v)[x] = p;
            if (!binary) {
                (*r)[x] = q;
            }
            last_x = x;
        }
        *p = y;
        if (!binary) {
            *q = z;
        }
        
        p++;
        if (!binary) {
            q++;
        }
    }
    
    *n = malloc(N * sizeof(int));
    for (x = 0; x < N - 1; x++) {
        (*n)[x] = (*v)[x + 1] - (*v)[x];
    }
    (*n)[x] = p - (*v)[x];
}

recdata_t* recdata_simple_create(FILE* user_data, FILE* item_data, int N_users, int N_items, int N_prefs, int binary) {
    recdata_t* recdata;
    recdata_simple_args_t* args;
    
    recdata = malloc(sizeof(recdata_t));
    recdata->N_users = N_users;
    recdata->N_items = N_items;
    recdata->N_prefs = N_prefs;
    
    recdata->userdata_size = recdata_simple_userdata_size;
    recdata->itemdata_size = recdata_simple_itemdata_size;
    recdata->userdata = recdata_simple_userdata;
    recdata->itemdata = recdata_simple_itemdata;
    recdata->close = recdata_simple_close;
    
    args = malloc(sizeof(recdata_simple_args_t));
    recdata->args = args;
    
    args->binary = binary;
    read_data(&(args->n_userdata), &(args->i_userdata), &(args->r_userdata), user_data, N_users, N_prefs, binary);
    
    read_data(&(args->n_itemdata), &(args->i_itemdata), &(args->r_itemdata), item_data, N_items, N_prefs, binary);
    
    return recdata;    
}

