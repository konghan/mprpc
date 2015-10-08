

#ifndef __REDIS_RPC_GEN_H__
#define __REDIS_RPC_GEN_H__

#ifdef __cpluscplus
extern "C" {
#endif


struct redisApi_service{

    char *lget(void *inst, char *key);
    int lset(void *inst, char *key, char *val);
    
    int mset(void *inst, fixarray_t *reqs);     // reqs - list<redis_req>
    fixarray_t *mget(void *inst, fixarray_t *reqs); 
    // reqs - list<redis_req>; return - list<redis_rsp>
};


extern int redisApi_on_accepted(void *inst, neurpc_ses_t *ses);
extern int redisApi_on_closed(void *inst, neurpc_ses_t *ses);

extern char *redisApi_lget(void *inst, char *key);
extern int redisApi_lset(void *inst, char *key, char *val);
    
extern int redisApi_mset(void *inst, fixarray_t *reqs);     // reqs - list<redis_req>
extern fixarray_t *redisApi_mget(void *inst, fixarray_t *reqs); 

#ifdef __cpluscplus
}
#endif

#endif // __REDIS_RPC_GEN_H__

