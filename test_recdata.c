#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "aux.h"

int main(int argc, char** argv) {
    FILE* user_data;
    FILE* item_data;
    int N_users;
    int N_items;
    int N_prefs;
    int binary;
    recdata_t* recdata;

    if (!read_args(argc, argv, &user_data, &item_data, &N_users, &N_items, &N_prefs, &binary)) {
        return EXIT_FAILURE;
    }

    recdata = recdata_simple_create(user_data, item_data, N_users, N_items, N_prefs, binary);
    
    fclose(user_data);
    fclose(item_data);
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
