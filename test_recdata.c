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
    int uid;
    int iid;
    int n;
    int i;
    int* v;
    
    (void) argc;
    (void) argv;

    user_data = fopen("ml1M_data/total_u.txt", "r");
    item_data = fopen("ml1M_data/total_i.txt", "r");
    N_users = 6040;
    N_items = 3706;
    N_prefs = 1000209;

    recdata = recdata_simple_create(user_data, item_data, N_users, N_items, N_prefs);
    
    fclose(user_data);
    fclose(item_data);
    
    printf("%d users\n", recdata->N_users);
    printf("%d items\n", recdata->N_items);
    printf("%d prefs\n", recdata->N_prefs);
    
    uid = 6039;
    n = recdata_n_items(recdata, uid);
    printf("user %d has %d items\n", uid, n);
    v = recdata_v_items(recdata, uid);
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    
    iid = 3705;
    n = recdata_n_users(recdata, iid);
    printf("item %d has %d users\n", iid, n);
    v = recdata_v_users(recdata, iid);
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
