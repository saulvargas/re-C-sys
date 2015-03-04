#include <stdlib.h>
#include "topn.h"

struct topn_ {
    int* keys;
    double* values;
    int capacity;
    int size;
};

topn_t* topn_create(int capacity) {
    topn_t* topn = malloc(sizeof(topn_t));

    topn->capacity = capacity;
    topn->keys = calloc(capacity, sizeof(int));
    topn->values = calloc(capacity, sizeof(double));
    topn->size = 0;
    
    return topn;
}

void topn_close(topn_t* topn) {
    free(topn->keys);
    free(topn->values);
    free(topn);
}

void topn_clear(topn_t* topn) {
    topn->size = 0;
}

void swap(topn_t* topn, int i, int j) {
    int k;
    double v;

    k = topn->keys[i];
    topn->keys[i] = topn->keys[j];
    topn->keys[j] = k;
    v = topn->values[i];
    topn->values[i] = topn->values[j];
    topn->values[j] = v;
}

void reverse(topn_t* topn) {
    int i = 0;
    int j = topn->size - 1;
    while (j > i) {
        swap(topn, i, j);
        j--;
        i++;
    }
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return (i + 1) * 2 - 1;
}

int right(int i) {
    return (i + 1) * 2;
}

int minChild(topn_t* topn, int i) {
    int l = left(i);
    int r = right(i);

    if (l >= topn->capacity) {
        return -1;
    }

    if (r >= topn->size) {
        return l;
    }

    return (topn->values[l] < topn->values[r] || (topn->values[l] == topn->values[r] && topn->keys[l] < topn->keys[r])) ? l : r;
}

void topn_sort(topn_t* topn) {
    int i;
    int j;

    int origSize = topn->size;
    int origCapacity = topn->capacity;
    topn->capacity = topn->size;
    while (topn->size > 0) {
        swap(topn, 0, topn->size - 1);
        topn->size--;
        topn->capacity--;
        i = 0;
        j = minChild(topn, i);
        while (j != -1 && (topn->values[i] > topn->values[j] || (topn->values[i] == topn->values[j] && topn->keys[i] > topn->keys[j]))) {
            swap(topn, i, j);
            i = j;
            j = minChild(topn, i);
        }
    }
    topn->size = origSize;
    topn->capacity = origCapacity;
    reverse(topn);
}

int topn_size(topn_t* topn) {
    return topn->size;
}

int topn_is_empty(topn_t* topn) {
    return topn->size == 0;
}

int topn_add_all(topn_t* topn, int n, int* keys, double* values) {
    int i;
    int c;
    
    c = 0;
    for (i = 0; i < n; i++) {
        c += topn_add(topn, keys[i], values[i]);
    }
    
    return c;
}

int topn_add(topn_t* topn, int key, double value) {
    int i;
    int j;

    if (topn->size < topn->capacity) {
        i = topn->size;
        j = parent(i);
        topn->keys[i] = key;
        topn->values[i] = value;
        while (i > 0 && (topn->values[j] > topn->values[i] || (topn->values[j] == topn->values[i] && topn->keys[j] > topn->keys[i]))) {
            swap(topn, i, j);
            i = j;
            j = parent(i);
        }
        topn->size++;

        return 1;
    } else {
        if (topn->values[0] > value || (topn->values[0] == value && topn->keys[0] > key)) {
            return 0;
        }
        i = 0;
        j = minChild(topn, i);
        topn->keys[i] = key;
        topn->values[i] = value;
        while (j != -1 && (topn->values[i] > topn->values[j] || (topn->values[i] == topn->values[j] && topn->keys[i] > topn->keys[j]))) {
            swap(topn, i, j);
            i = j;
            j = minChild(topn, i);
        }

        return 1;
    }
}

int* topn_get_keys(topn_t* topn) {
    return topn->keys;
}

double* topn_get_values(topn_t* topn) {
    return topn->values;
}
