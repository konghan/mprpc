
#include "neuworker.h"
#include "logger.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

static int neumap_event_compare(void *kf, fixmap_node_t *ks){
    int efd = (int)kf;
    neumap_event_t *nes = container_of(ks, neumap_event_t, fmnode);

    if(efd > nes->efd){
        return 1;
    }else if(efd < nes->efd){
        return -1;
    }else{
        return 0;
    }
}

static void neuworker_dotask(evutil_socket_t sock, short act, void *data){
    neuworker_t *nw = (neuworker_t *)data;
    neutask_t task;
    int ret;

    if(act & EV_READ){
        ret = read(sock, &task, sizeof(task));
        if(ret != sizeof(task)){
            // FIXME:
            return;
        }
        task.dotask(nw, task.data);
    }
}

int neuworker_init(neuworker_t *nw, void *data){
    ASSERT(nw != NULL);

    nw->base = event_base_new();
    if(!nw->base){
        log_warn("neuworker : event base new fail\n");
        return -1;    
    }

    if(evutil_socketpair(AF_LOCAL, SOCK_STREAM, 0, nw->socks) != 0){
        event_base_free(nw->base);
        log_warn("neuworker : socket pair fail\n");
        return -1;
    }
    evutil_make_socket_nonblocking(nw->socks[0]);
    evutil_make_socket_nonblocking(nw->socks[1]);

    nw->evsock = event_new(nw->base, nw->socks[0], EV_READ, neuworker_dotask, nw);
    if(nw->evsock == NULL){
        event_base_free(nw->base);
        evutil_closesocket(nw->socks[0]);
        evutil_closesocket(nw->socks[1]);
        log_warn("neuworker : event new fail\n");
        return -1;
    }
    event_add(nw->evsock, NULL);

    if(pthread_mutex_init(&nw->lock, NULL) != 0){
        log_warn("neuworker : inti mutex fail\n");
        return -1;
    }

    fixmap_init(&nw->fmevs, neumap_event_compare);

    nw->data = data;

    return 0;
}

int neuworker_fini(neuworker_t *nw){
    ASSERT(nw != NULL);

    if(!nw->base){
        log_warn("neuworker : neuworker already fini\n");
        return 0;
    }

    event_base_loopbreak(nw->base);
    event_base_free(nw->base);
    nw->base = NULL;

    evutil_closesocket(nw->socks[0]);
    evutil_closesocket(nw->socks[1]);

    fixmap_fini(&nw->fmevs);

    pthread_mutex_destroy(&nw->lock);

    return 0;
}

int neuworker_dispatch_task(neuworker_t *nw, neutask_t *task){
    int ret;

    ASSERT((nw != NULL) && (task != NULL));

    ret = pthread_mutex_trylock(&nw->lock);
    ret = write(nw->socks[1], task, sizeof(*task));
    pthread_mutex_unlock(&nw->lock);

    return (ret == sizeof(*task)) ? 0 : -1;
}

static void watch_event_routine(neuworker_t *nw, void *data){
    neuevent_t *e = (neuevent_t *)data;
    neumap_event_t *ne = xmalloc(sizeof(*ne));
    if(ne == NULL){
        e->onevent(e->efd, NEUEVENT_FLAGS_ENOMEM, e->data);
        xfree(e);
        log_warn("neuworker : no more memory\n");
        return ;
    }

    ne->efd = e->efd;
    ne->data = e->data;
    ne->ev = event_new(nw->base, ne->efd, e->flags, e->onevent, e->data);
    if(ne->ev == NULL){
        e->onevent(e->efd, NEUEVENT_FLAGS_EEVFAIL, e->data);
        xfree(e);
        xfree(ne);
        log_warn("neuworker : event new fail\n");
        return ;
    }
    event_add(ne->ev, NULL);

    fixmap_node_init(&ne->fmnode);
    fixmap_insert(&nw->fmevs, (void*)ne->efd, &ne->fmnode);
    xfree(e);
}

static void close_event_routine(neuworker_t *nw, void *data){
    neuevent_t *e = (neuevent_t *)data;
    fixmap_node_t *node;
    neumap_event_t *ne;

    if(fixmap_remove(&nw->fmevs, (void *)e->efd, &node) != 0){
        log_warn("neuworker : remove efd not exist\n");
        return ;
    }

    ne = container_of(node, neumap_event_t, fmnode);
    ne->onevent(ne->efd, NEUEVENT_FLAGS_CLOSE, ne->data);
    xfree(ne);
}

static inline int dispatch_event_task(neuworker_t *nw, neuevent_t *ev, neutask_routine nr){
    neutask_t   t;
    neuevent_t *e;
    int ret;

    e = xmalloc(sizeof(*ev));
    if(e == NULL)
        return -ENOMEM;

    memcpy(e, ev, sizeof(*ev));

    t.data = e;
    t.dotask = nr;

    ret = neuworker_dispatch_task(nw, &t);

    return ret;
}

int neuworker_watch_event(neuworker_t *nw, neuevent_t *ev){
    return dispatch_event_task(nw, ev, watch_event_routine);
}

int neuworker_close_event(neuworker_t *nw, neuevent_t *ev){
    return dispatch_event_task(nw, ev, close_event_routine);
}

