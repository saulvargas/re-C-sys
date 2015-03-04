#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "similarity.h"
#include "topn.h"

int main(int argc, char** argv) {
    FILE* user_data;
    FILE* item_data;
    int N_users;
    int N_items;
    int N_prefs;
    recdata_t* recdata;
    int uid;
    int i;
    similarity_t* similarity;
    idpairs_t* pairs;

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

    similarity = similarity_cosine0_create(recdata);

    for (uid = 0; uid < recdata->N_users; uid++) {
        pairs = similarity_calculate(similarity, uid, 10);

        for (i = 0; i < pairs->n; i++) {
            printf("%d\t%d\t%.4f\n", uid, pairs->keys[i], pairs->values[i]);
        }
        
        free(pairs->keys);
        free(pairs->values);
        free(pairs);
    }
 
    similarity_close(similarity);
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
