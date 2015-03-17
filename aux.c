#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_args(int argc, char** argv, FILE** user_data, FILE** item_data, int* N_users, int* N_items, int* N_prefs, int* binary) {
    char* file;

    if (argc != 6) {
        return 0;
    }
    
    file = malloc((strlen(argv[1]) + 7) * sizeof(char));

    strcpy(file, argv[1]);
    strcat(file, "_u.txt");
    *user_data = fopen(file, "r");

    strcpy(file, argv[1]);
    strcat(file, "_i.txt");
    *item_data = fopen(file, "r");
    
    free(file);

    *N_users = atoi(argv[2]);
    *N_items = atoi(argv[3]);
    *N_prefs = atoi(argv[4]);
    *binary = atoi(argv[5]);
    
    return 1;
}

