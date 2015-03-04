#ifndef TOPN_H
#define TOPN_H

#include <stdlib.h>

typedef struct topn_ topn_t;

topn_t* topn_create(int capacity);

void topn_close(topn_t* topn);

void topn_clear(topn_t* topn);

void topn_sort(topn_t* topn);

int topn_size(topn_t* topn);

int topn_is_empty(topn_t* topn);

int topn_add(topn_t* topn, int key, double value);

int topn_add_all(topn_t* topn, int n, int* keys, double* values);

int* topn_get_keys(topn_t* topn);

double* topn_get_values(topn_t* topn);

#endif
