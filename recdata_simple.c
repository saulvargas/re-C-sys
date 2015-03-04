#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "recdata.h"

typedef struct {
    int* n_items;
    int** v_items;
    int* n_users;
    int** v_users;
} recdata_simple_args_t;

int recdata_simple_n_items(int uid, void* args) {
    return ((recdata_simple_args_t*) args)->n_items[uid];
}

int* recdata_simple_v_items(int uid, void* args) {
    return ((recdata_simple_args_t*) args)->v_items[uid];
}

int recdata_simple_n_users(int iid, void* args) {
    return ((recdata_simple_args_t*) args)->n_users[iid];
}

int* recdata_simple_v_users(int iid, void* args) {
    return ((recdata_simple_args_t*) args)->v_users[iid];
}

void recdata_simple_close(void* args) {
    recdata_simple_args_t* args2 = args;
    
    free(args2->n_items);
    free(args2->v_items[0]);
    free(args2->v_items);
    free(args2->n_users);
    free(args2->v_users[0]);
    free(args2->v_users);
    free(args2);
}

#define MAX_LEN_LINE 256

void read_data(int** n, int*** v, FILE* data, int N, int N_prefs) {
    char line[MAX_LEN_LINE];
    int last_x;
    int x;
    int y;
    int* p;

    *v = malloc(N * sizeof(int*));
    p = malloc(N_prefs * sizeof(int));
    last_x = -1;
    while (fgets(line, MAX_LEN_LINE, data) != NULL) {
        x = atoi(strtok(line, "\t"));
        y = atoi(strtok(NULL, "\t"));
        
        if (x != last_x) {
            (*v)[x] = p;
            last_x = x;
        }
        *p = y;
       
        p++;
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
    
    recdata->n_items = recdata_simple_n_items;
    recdata->v_items = recdata_simple_v_items;
    recdata->n_users = recdata_simple_n_users;
    recdata->v_users = recdata_simple_v_users;
    recdata->close = recdata_simple_close;
    
    args = malloc(sizeof(recdata_simple_args_t));
    recdata->args = args;
    
    read_data(&(args->n_items), &(args->v_items), user_data, N_users, N_prefs);
    
    read_data(&(args->n_users), &(args->v_users), item_data, N_items, N_prefs);
    
    return recdata;    
}

