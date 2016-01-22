

#ifndef __NEUWORKER_H__
#define __NEUWORKER_H__

#include "osapi.h"
#include "fixmap.h"

#include <event2/event.h>
#include <pthread.h>

#ifdef __cpluscplus
extern "C" {
#endif

#define NEUEVENT_FLAGS_CLOSE            0x0100
#define NEUEVENT_FLAGS_ENOMEM           0x1000
#define NEUEVENT_FLAGS_EEVFAIL          0x2000

struct neuworker;

typedef void (*neutask_routine)(struct neuworker *w, void *data);
typedef struct neutask {
    // public data
    void *data;
    neutask_routine dotask;
}neutask_t;
#define NEUTASK_INIT(task_, data_, dotask_)    do{      \
        (task_)->data = data_;                          \
        (task_)->dotask = dotask_;                      \
}while(0)

typedef void (*neuevent_on_event)(evutil_socket_t efd, short flags, void *data);
typedef struct neuevent {
    // public data
    void *data;

    neuevent_on_event  onevent;
    int efd;
    int flags;      // libevent epoll flags
    struct timeval tv;
}neuevent_t;
#define NEUEVENT_INIT(ev_, data_, onev_, efd_, flags_, tv_)     do{         \
    (ev_)->data = data_;    \
    (ev_)->onevent = onev_; \
    (ev_)->efd = efd_;      \
    (ev_)->flags = flags_;  \
    (ev_)->tv = tv_;        \
}while(0)


typedef struct neumap_event{
    fixmap_node_t fmnode;
    
    int efd;
    struct event *ev;

    neuevent_on_event onevent;
    void *data;
}neumap_event_t;

typedef struct neuworker{
    // public data
    void *data;

    // private data
    struct event_base *base;

    fixmap_t fmevs;

    pthread_mutex_t  lock;
    struct event *evsock;
    evutil_socket_t  socks[2];
}neuworker_t;

int neuworker_init(neuworker_t *nw, void *data);
int neuworker_fini(neuworker_t *nw);

int neuworker_dispatch_task(neuworker_t *nw, neutask_t *task);

int neuworker_watch_event(neuworker_t *nw, neuevent_t *ev);
int neuworker_close_event(neuworker_t *nw, neuevent_t *ev);

static inline int neuworker_dispatch(neuworker_t *nw){
    if((nw != NULL) && (nw->base != NULL))
        return event_base_dispatch(nw->base);

    return -1;
}


#ifdef __cpluscplus
}
#endif

#endif // __NEUWORKER_H__

