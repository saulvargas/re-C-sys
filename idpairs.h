#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct idpairs_ idpairs_t;

idpairs_t* idpairs_create(int n, int* keys, double* values);

void idpairs_close_deep(idpairs_t* pairs);

void idpairs_close_shallow(idpairs_t* pairs);

int idpairs_size(idpairs_t* pairs);

int* idpairs_keys(idpairs_t* pairs);

double* idpairs_values(idpairs_t* pairs);

void idpairs_revert(idpairs_t* pairs);

#endif
