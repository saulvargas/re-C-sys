#ifndef TOPN_H
#define TOPN_H

#include <stdlib.h>
#include "idpairs.h"

typedef struct topn_ topn_t;

topn_t* topn_create(int capacity);

void topn_close(topn_t* topn);

void topn_clear(topn_t* topn);

void topn_sort(topn_t* topn);

int topn_size(topn_t* topn);

int topn_is_empty(topn_t* topn);

int topn_add(topn_t* topn, int key, double value);

idpairs_t* topn_get_pairs(topn_t* topn);

#endif
