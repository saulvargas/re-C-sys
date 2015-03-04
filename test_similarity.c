#include <stdlib.h>
#include <stdio.h>
#include "recdata.h"
#include "similarity.h"

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
    sim_t* sim;

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
        sim = similarity_calculate(similarity, uid);
        for (i = 0; i < sim->n; i++) {
            printf("%d\t%d\t%.4f\n", uid, sim->uids[i], sim->sims[i]);
        }
        free(sim->uids);
        free(sim->sims);
        free(sim);
    }
 
    similarity_close(similarity);
    
    recdata_close(recdata);
    
    return EXIT_SUCCESS;
}
