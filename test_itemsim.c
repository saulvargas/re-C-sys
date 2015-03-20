#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "similarity.h"
#include "topn.h"
#include "aux.h"

int main(int argc, char** argv) {
    FILE* user_data;
    FILE* item_data;
    int N_users;
    int N_items;
    int N_prefs;
    int binary;
    recdata_t* recdata;
    recdata_t* cerdata;
    int iid;
    int i;
    similarity_t* similarity;
    idpairs_t* pairs;

    if (!read_args(argc, argv, &user_data, &item_data, &N_users, &N_items, &N_prefs, &binary)) {
        return EXIT_FAILURE;
    }

    recdata = recdata_simple_create(user_data, item_data, N_users, N_items, N_prefs, binary);
    cerdata = recdata_transposed_create(recdata);
    
    fclose(user_data);
    fclose(item_data);

    if (!binary) {
        similarity = similarity_veccosine_create(cerdata);
    } else {
        similarity = similarity_setcosine_create(cerdata);
    }

    for (iid = 0; iid < recdata->N_items; iid++) {
        pairs = similarity_calculate(similarity, iid, 10);

        for (i = 0; i < idpairs_size(pairs); i++) {
            printf("%d\t%d\t%.4f\n", iid, idpairs_keys(pairs)[i], idpairs_values(pairs)[i]);
        }
        
        idpairs_close_deep(pairs);
    }
 
    similarity_close(similarity);
    
    recdata_close(recdata);
    recdata_close(cerdata);
   
    return EXIT_SUCCESS;
}
