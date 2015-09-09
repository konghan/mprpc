
#include "redis-rpc-gen.h"


static inline int redisApi_rpc_lget(void *inst, neupdu_t *req, neupdu_t **rsp){
    struct redisApi_lget_req *q;
    struct redisApi_lget_rsp p;

    if(redisApi_lget_req_unmashal(req, &q) != 0){
        return -1;
    }
    
    p.ret = redisApi_lget(inst, q->p0);

    redisApi_lget_req_free(q);
 
    if(redisApi_lget_rsp_mashal(p, rsp) != 0){
        return -1;
    }

    // p is not point, free memory hold by p
    redisApi_lget_rsp_free(&p);

    return 0;
}

static inline int redisApi_rpc_lset(void *inst, neupdu_t *req, neupdu_t **rsp){
    struct redisApi_lset_req *q;
    struct redisApi_lset_rsp p;

    if(redisApi_lset_req_unmashal(req, &q) != 0){
        return -1;
    }
    
    p.ret = redisApi_lset(inst, q->p0);

    redisApi_lset_req_free(q); 

    if(redisApi_lset_rsp_mashal(rsp, r) != 0){
        return -1;
    }

    redisApi_lset_rsp_free(&p);

    return 0;;
}
 
static inline int redisApi_rpc_mset(void *inst, neupdu_t *req, neupdu_t **rsp){
    struct redisApi_mset_req *q;
    struct redisApi_mset_rsp p;

    if(redisApi_mset_req_unmashal(req, &q) != 0){
        return -1;
    }
    
    p.ret = redisApi_mset(inst, q->p0);

    redisApi_mset_req_free(q); 

    if(redisApi_mset_rsp_mashal(rsp, r) != 0){
        return -1;
    }

    redisApi_mset_rsp_free(&p);

    return 0;;
}

static inline int redisApi_rpc_mget(void *inst, neupdu_t *req, neupdu_t **rsp){
    struct redisApi_mget_req *q;
    struct redisApi_mget_rsp p;

    if(redisApi_mget_req_unmashal(req, &q) != 0){
        return -1;
    }
    
    p.ret = redisApi_mget(inst, q->p0);

    redisApi_mget_req_free(q); 

    if(redisApi_mget_rsp_mashal(rsp, r) != 0){
        return -1;
    }

    redisApi_mget_rsp_free(&p);

    return 0;;
}

static int redisApi_rpc_dispatch(nueses_t *ses, neupdu_t *req, neupdu_t **rsp)
    int callidx = req->callidx;

    switch(callidx){
    case 0:
        return redisApi_rpc_lget(ses->rpc_inst, req, rsp);

    case 1:
        return redisApi_rpc_lset(ses->rpc_inst, req, rsp);

    case 2:
        return redisApi_rpc_mset(ses->rpc_inst, req, rsp);

    case 4:
        return redisApi_rpc_mget(ses->rpc_inst, req, rsp);

    default:
        break;
    }
}

static int redisApi_on_accepted_stub(neurpc_t *rpc, neurpc_ses_t *ses){
    ses->dispatch_rpc = redisApi_rpc_dispatch;
    ses->on_closed = redisApi_on_closed;

    return redisApi_on_accepted(rpc->rpc_inst, ses);
}

static int redis_on_shutdown_stub(neurpc_t *rpc){
    return redis_on_shutdown(rpc->rpc_inst);
}

static int redis_on_listen_stub(neurpc_t *rpc){
    return redis_on_listen(rpc->rpc_inst);
}

int redisApi_rpc_init(neuservice_t *ns, void *inst){
    neurpc_t *rpc = neurpc_new();
    if(rpc == NULL)
        return -1;

    rpc->rpc_service = "redisApi";
    rpc->rpc_inst = inst;

    rpc->on_listen = redis_on_listen_stub;
    rpc->on_shutdown = redis_on_shutdown_stub;

    rpc->on_accepted = redisApi_on_acepted_stub;

    return neuservice_regist_rpc(ns, rpc);
}


