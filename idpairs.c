#include <stdlib.h>
#include "idpairs.h"

struct idpairs_ {
    int n;
    int* keys;
    double* values;
};

idpairs_t* idpairs_create(int n, int* keys, double* values) {
    idpairs_t* pairs = malloc(sizeof(idpairs_t));
    
    pairs->n = n;
    pairs->keys = keys;
    pairs->values = values;
    
    return pairs;
}

void idpairs_close(idpairs_t* pairs) {
    free(pairs->keys);
    free(pairs->values);
    free(pairs);
}

int idpairs_size(idpairs_t* pairs) {
    return pairs->n;
}

int* idpairs_keys(idpairs_t* pairs) {
    return pairs->keys;
}

double* idpairs_values(idpairs_t* pairs) {
    return pairs->values;
}

void revert_int(int n, int* v) {
    int i;
    int x;
    for (i = 0; i < n / 2; i++) {
        x = v[i];
        v[i] = v[n - i - 1];
        v[n - i - 1] = x;
    }
}

void revert_double(int n, double* v) {
    int i;
    double x;
    for (i = 0; i < n / 2; i++) {
        x = v[i];
        v[i] = v[n - i - 1];
        v[n - i - 1] = x;
    }

}

void idpairs_revert(idpairs_t* pairs) {
    revert_int(pairs->n, pairs->keys);
    revert_double(pairs->n, pairs->values);
}

