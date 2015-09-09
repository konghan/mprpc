
#include "redis-rpc-gen.h"

char *redisApi_lget(void *inst, char *key){
}

int redisApi_lset(void *inst, char *key, char *val){
}
    
int redisApi_mset(void *inst, fixarray_t *reqs){
}

fixarray_t *redisApi_mget(void *inst, fixarray_t *reqs){
}

// session 
int redisApi_on_accepted(void *inst, neurpc_ses_t *ses){
}

int redisApi_on_closed(void *inst, neurpc_ses_t *ses){
}

// service
int redisApi_on_listen(void *inst){
}

int redisApi_on_shutdown(void *inst){
}

