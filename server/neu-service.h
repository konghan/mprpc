

#ifndef __NEUSERVICE_H__
#define __NEUSERVICE_H__

#include "neu-session.h"
#include "fixarray.h"

#include <event2/listener.h>
#include <sys/socket.h>

#ifdef __cpluscplus
extern "C" {
#endif

struct neuservice;
typedef struct neuservice neuservice_t;
struct neumsg;
struct neuworker;

typedef void (*neuservice_on_setup_session)(neuservice_t *ns, neusession_t *ses, void *data);
typedef void (*neuservice_on_close_session)(neuservice_t *ns, neusession_t *ses, void *data);
typedef void (*neuservice_on_shutdown)(neuservice_t *ns, void *data);

typedef struct neuservice{
    // public 
    void *ns_data;

    // private
    struct neuworker *ns_worker;
    struct sockaddr_in ns_addr;

    neuservice_on_setup_session  on_setup;
    neuservice_on_close_session  on_close;
    neuservice_on_shutdown       on_shutdown;

    fixarray_t ns_msgs;
    fixarray_t ns_rpcs;
    fixarray_t ns_msqs;

    // init in worker thread
    struct evconnlistener *ns_listen;
}neuservice_t;

int neuservice_init(neuservice_t *ns, struct sockaddr_in *addr, 
        neuservice_on_setup_session setupcb,
        neuservice_on_close_session closecb, 
        neuservice_on_shutdown shutdowncb, void*data);

int neuservice_fini(neuservice_t *ns);

int neuservice_regist_msg(neuservice_t *ns, struct neumsg *msg);
//int neuservice_regist_rpc(neuservice_t *ns, neurpc_t *rpc);
//int neuservice_regist_msq(neuservice_t *ns, neumsq_t *msq);

#ifdef __cpluscplus
}
#endif

#endif // __NEUSERVICE_H__

