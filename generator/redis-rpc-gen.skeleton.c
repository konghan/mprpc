
#include "redis-rpc-gen.h"

char *redisApi_lget(void *inst, char *key){
    return NULL;
}

int redisApi_lset(void *inst, char *key, char *val){
    return -1;
}
    
int redisApi_mset(void *inst, fixarray_t *reqs){
    return -1;
}

fixarray_t *redisApi_mget(void *inst, fixarray_t *reqs){
    return -1;
}

// session 
int redisApi_on_accepted(void *inst, neurpc_ses_t *ses){
    return -1;
}

int redisApi_on_closed(void *inst, neurpc_ses_t *ses){
    return -1;
}

// service
int redisApi_on_listen(void *inst){
    return -1;
}

int redisApi_on_shutdown(void *inst){
    return -1;
}

