#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "similarity.h"
#include "topn.h"
#include "recommender.h"
#include "aux.h"

int main(int argc, char** argv) {
    FILE* user_data;
    FILE* item_data;
    int N_users;
    int N_items;
    int N_prefs;
    int binary;
    recdata_t* recdata;
    int uid;
    int i;
    similarity_t* similarity;
    recommender_t* recommender;
    idpairs_t* pairs;

    if (!read_args(argc, argv, &user_data, &item_data, &N_users, &N_items, &N_prefs, &binary)) {
        return EXIT_FAILURE;
    }

    recdata = recdata_simple_create(user_data, item_data, N_users, N_items, N_prefs, binary);
    
    fclose(user_data);
    fclose(item_data);

    if (!binary) {
        similarity = similarity_veccosine_create(recdata);
    } else {
        similarity = similarity_setcosine_create(recdata);
    }

    recommender = recommender_ub_create(recdata, similarity, 100, binary);

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
