

#ifndef __NEUCLI_H__
#define __NEUCLI_H__

#include "fixarray.h"

#include <event2/listener.h>
#include <sys/socket.h>

#ifdef __cpluscplus
extern "C" {
#endif

struct neusrv;
struct neurpc;

int neusrv_init(struct neusrv *srv, void *data);
int neusrv_fini(struct neusrv *srv);

int neusrv_setsocket(struct neusrv *srv, int sock);

/** message

struct example{};
struct example_rsp{};

// generate skeleton for user
extern void example_cb_impl(void *data, uint32_t seq, struct example_rsp *rsp);

static void example_rsp(void *data, uint32_t seq, struct neupdu *rsp){
    struct example_rsp *rsp = example_rsp_unmashal(rsp);
    
    example_cb_impl(data, seq, rsp);
    
    example_rsp_free(rsp);
}

int send_example(struct neusrv *srv, struct example *ex, void *data, uint32_t *seq){
    struct neupdu *req = example_mashal(ex);
    int ret;

    ret = send_example(srv, req, example_rsp, data, seq);
    neupdu_free(req);

    return ret;
} 
*/
typedef void (*neusrv_rspcb)(void *data, uint32_t seq, struct neupdu *rsp);
// message name send()
int neusrv_send_message(struct neusrv *srv, struct neupdu *req,
                        neusrv_rspcb cb, void *data, uint32_t *seq);

/** message queue
*/
typedef void (*neusrv_msqcb)(struct neusrv *srv, void *data, struct neupdu *msg);
int neusrc_subscribe(struct neusrv *srv, const char *msq, neusrv_msqcb cb, void *data); 
int neusrc_unsubscribe(struct neusrv *srv, const char *msq);


/** RPC
*/

int neusrv_setup_rpc(struct neusrv *srv, const char *service, struct neurpc **rpc);
int neusrv_close_rpc(struct neurpc *rpc);

/*
// sync mode
struct outs* neurpc_example(struct neurpc *rpc, struct example *e, struct sample *s){
    struct __example_param *ep = {e, s};
    struct neupdu *req = __example_mashal(ep);
    struct neupdu *rsp = NULL;
    struct outs *o;
    int ret;

    // func -- generate by compiler
    ret = neurpc_call(rpc, func, req, &rsp);
    neupdu_free(req);
    if(ret != 0){
        return NULL;
    }

    o = __example_outs_unmashal(rsp); 
    neupdu_free(rsp);
    return o;
}

// async mode
typedef void (*neurpc_examplecb)(void *data, uint32_t seq, struct outs *o);
void example_cb(void *data, int seq, struct outs *o){
    //
    // user implememt code here
    //
}

static void neurpc_example_callback(struct neurpc *rpc, uint32_t seq, void *data,
                    struct neupdu *rsp){
    struct __example_cbdata *ec = (struct __example_cbdata*)data;
    struct outs *o = __outs_unmashal(rsp);

    ec->cb(ec->data, seq, o);
}

uint32_t neurpc_example(struct neurpc *rpc, struct example *e, struct sample *s,
        neurpc_examplecb cb, void *data){
    struct __example_param ep = {e, s};
    struct __example_cbdata ec = {example_cb, data};
    struct neupdu *req = __example_mashal(&ep);
    uint32_t ret;

    ret = neurpc_call_async(rpc, func, req, neurpc_example_callback, &ec);
    neupdu_free(req);

    return ret;
}

*/

typedef void (*neurpc_cb)(struct neurpc *rpc, unint32_t seq, void *data, struct neupdu *rsp);
int neurpc_call(struct neurpc *rpc, int func, struct neupdu *req, struct neupdu **rsp);
uint32_t neurpc_call_async(struct neurpc *rpc, int func, struct neupdu *req, 
            neurpc_cb cb, void *data);

#ifdef __cpluscplus
}
#endif

#endif // __NEUCLI_H__

