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
    return idpairs_create(a->n_userdata[uid], a->i_userdata[uid], a->r_userdata[uid]);
}

idpairs_t* recdata_simple_itemdata(int iid, void* args) {
    recdata_simple_args_t* a = args;
    return idpairs_create(a->n_itemdata[iid], a->i_itemdata[iid], a->r_itemdata[iid]);
}

void recdata_simple_close(void* args) {
    recdata_simple_args_t* args2 = args;
    
    free(args2->n_userdata);
    free(args2->i_userdata[0]);
    free(args2->i_userdata);
    free(args2->r_userdata[0]);
    free(args2->r_userdata);
    free(args2->n_itemdata);
    free(args2->i_itemdata[0]);
    free(args2->i_itemdata);
    free(args2->r_itemdata[0]);
    free(args2->r_itemdata);
    free(args2);
}

#define MAX_LEN_LINE 256

void read_data(int** n, int*** v, double*** r, FILE* data, int N, int N_prefs) {
    char line[MAX_LEN_LINE];
    int last_x;
    int x;
    int y;
    double z;
    int* p;
    double* q;

    *v = malloc(N * sizeof(int*));
    *r = malloc(N * sizeof(double*));
    p = malloc(N_prefs * sizeof(int));
    q = malloc(N_prefs * sizeof(double));
    last_x = -1;
    while (fgets(line, MAX_LEN_LINE, data) != NULL) {
        x = atoi(strtok(line, "\t"));
        y = atoi(strtok(NULL, "\t"));
        z = atof(strtok(NULL, "\t"));
        
        if (x != last_x) {
            (*v)[x] = p;
            (*r)[x] = q;
            last_x = x;
        }
        *p = y;
        *q = z;
       
        p++;
        q++;
    }
    
    *n = malloc(N * sizeof(int));
    for (x = 0; x < N - 1; x++) {
        (*n)[x] = (*v)[x + 1] - (*v)[x];
    }
    (*n)[x] = p - (*v)[x];
}

recdata_t* recdata_simple_create(FILE* user_data, FILE* item_data, int N_users, int N_items, int N_prefs) {
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
    
    read_data(&(args->n_userdata), &(args->i_userdata), &(args->r_userdata), user_data, N_users, N_prefs);
    
    read_data(&(args->n_itemdata), &(args->i_itemdata), &(args->r_itemdata), item_data, N_items, N_prefs);
    
    return recdata;    
}

