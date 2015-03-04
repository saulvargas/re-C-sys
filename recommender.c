#include <stdlib.h>
#include "aux.h"
#include "recommender.h"

idpairs_t* recommender_recommend(recommender_t* recommender, int uid, int n) {
    return recommender->recommend(uid, n, recommender->args);
}

void recommender_close(recommender_t* recommender) {
    recommender->close(recommender->args);
    
    free(recommender);
}

