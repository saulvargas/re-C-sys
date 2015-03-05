#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"

int main(int argc, char** argv) {
    FILE* user_data;
    FILE* item_data;
    int N_users;
    int N_items;
    int N_prefs;
    recdata_t* recdata;

    if (argc != 4) {
        return EXIT_FAILURE;
    }    

    user_data = fopen("total_u.txt", "r");
    item_data = fopen("total_i.txt", "r");
    N_users = atoi(argv[1]);
    N_items = atoi(argv[2]);
    N_prefs = atoi(argv[3]);

    recdata = recdata_simple_create(user_data, item_data, N_users, N_items, N_prefs);
    
    fclose(user_data);
    fclose(item_data);
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
