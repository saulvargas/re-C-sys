#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "similarity.h"
#include "topn.h"
#include "recommender.h"

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
    recommender_t* recommender;
    idpairs_t* pairs;

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

    similarity = similarity_cosine0_create(recdata);

    recommender = recommender_ubknn_create(recdata, similarity, 100);

    for (uid = 0; uid < recdata->N_users; uid++) {
        pairs = recommender_recommend(recommender, uid, 100);

        for (i = 0; i < idpairs_size(pairs); i++) {
            printf("%d\t%d\t%.4f\n", uid, idpairs_keys(pairs)[i], idpairs_values(pairs)[i]);
        }
        
        idpairs_close_deep(pairs);
    }
 
    recommender_close(recommender);
 
    similarity_close(similarity);
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
