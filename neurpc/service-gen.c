
#include "service-gen.h"
#include "struct-gen.h"

static void rpc_dispatch(void *inst, void *service);

static  neurpc_service_t template_neurpc_svr = {
    .rpc_dispatch = rpc_dispatch,
    .rpc_on_accepted = template_accepted_callback,
    .rpc_on_closed = template_closed_callback,
}

static inline int rpc_func00(void *inst, neurpc_pdu_t *pdu, neurpc_pdu_t **rsp){
    struct template_rpc_func00_param *p;
    struct template_rpc_func00_rsp *r;

    if(rpc_func00_param_unmashal(pdu, &p) != 0){
        return -1;
    }
    
    *r = template_rpc_func00(inst, p->p0, p->p1, p->p2);

    if(r == NULL){
        return -1;
    }

    rpc_func00_rsp_mashal(rsp, r);

    return 0;
}

static int rpc_dispatch(void *inst, int callidx, neurpc_pdu_t *pdu, neurpc_pdu_t **rsp){
    template_service *svc = (template_service*)service;
    
    switch(callidx){
    case 0:
        return rpc_func00(inst, pdu, rsp);
        break;

    case 1:
        break;

    case 2:
        break;

    default:
        break;
    }
}

int template_service_init(neurpc_loop_t *loop, void *inst){
    template_neurpc_svr.rpc_service = "template_service";
    template_neurpc_svr.rpc_loop = loop;
    template_neurpc_svr.rpc_inst = inst;
    return neurpc_service_regist(loop, &template_neurpc_svr);
}

int template_service_fini(neurpc_loop_t *loop){
    return neurpc_service_unregist(loop, &template_neurpc_svr);
}


