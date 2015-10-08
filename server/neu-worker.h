
#include <event2/event.h>
#include <pthread.h>

#ifndef __NEU_WORKER_H__
#define __NEU_WORKER_H__

#ifdef __cpluscplus
extern "C" {
#endif

struct neuhandle;
struct neuservice;
struct neusession;

typedef struct neuworker{
    // public data
    void *data;

    // private data
    struct event_base *base;

    pthread_mutex_t  lock;
    struct event *evsock;
    evutil_socket_t  socks[2];
}neuworker_t;

int neuworker_init(neuworker_t *nw, void *data);
int neuworker_fini(neuworker_t *nw);

int neuworker_dispatch(neuworker_t *nw, struct neuhandle *nh);
int neuworker_watch_service(neuworker_t *nw, struct neuservice *ns);
int neuworker_watch_session(neuworker_t *nw, struct neusession *ns);

static inline int neuworker_run(neuworker_t *nw){
    if((nw != NULL) && (nw->base != NULL))
        return event_base_dispatch(nw->base);

    return -1;
}


#ifdef __cpluscplus
}
#endif

#endif // __NEU_WORKER_H__

