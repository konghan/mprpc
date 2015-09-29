
#include "neuloop.h"
#include "neuservice.h"

#ifndef __NEURPC_H__
#define __NEURPC_H__

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct neurpc_session{
    neuhandle_t seshandle;
    void *data;     // private data of this session

    // session info
    unsigned int seqnum;

    int dispatch_rpc(struct neurpc_ses_t *ses, neurpc_pdu_t *req, neurpc_pdu_t **rsp);
    
    int on_closed(void *data, struct neurpc_session *ses);
}neurpc_ses_t;

typedef struct neurpc{
    struct list_head node; // link to service

    char *rpc_service; // name of rpc service
    void *rpc_inst;    // instance private data
//    neurpc_loop_t *rpc_loop;

    void on_listen(void *inst);
    void on_shutdown(void *inst);

    int on_accepted(void *inst, neurpc_ses_t *ses);
}neurpc_t;

// int neurpc_setup(neurpc_t *svc);
// int neurpc_service_close(neurpc_service_t *svc);

#ifdef __cpluscplus
}
#endif

#endif // __NEURPC_H__

