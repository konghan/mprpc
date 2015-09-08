

#ifndef __NEURPC_H__
#define __NEURPC_H__

#ifdef __cpluscplus
extern "C" {
#endif

struct neurpc_session;
typedef struct neurpc_session neurpc_session_t;

struct neurpc_loop;
typedef struct neurpc_loop neurpc_loop_t;

struct neurpc_service{
    char *rpc_service;
    void *rpc_inst;
    neurpc_loop_t *rpc_loop;

    int rpc_dispatch(neurpc_session_t *ses, neurpc_pdu_t *req, neurpc_pdu_t **rsp);
    
    int rpc_on_listen(void *inst);
    int rpc_on_accepted(void *inst, neurpc_session_t *ses);
    int rpc_on_closed(void *inst, neurpc_session_t *ses);
    int rpc_on_shutdown(void *inst);
};
typedef struct neurpc_service neurpc_service_t;

int neurpc_service_setup(neurpc_service_t *svc);
int neurpc_service_close(neurpc_service_t *svc);

#ifdef __cpluscplus
}
#endif

#endif // __NEURPC_H__

