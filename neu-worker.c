
#include "neu-worker.h"

#include "neuron.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/socket.h>



static void neuworker_sockcb(evutil_socket_t sock, short act, void *data){
    neuhandle_t nh;
    neuworker_t *nw = (neuworker_t *)data;
    int ret;

    if(act & EV_READ){
        ret = read(sock, &nh, sizeof(nh));
        if(ret != sizeof(nh)){
            // FIXME:
            return;
        }

        nh.callback(nw, &nh, nh.data);
    }
}

int neuworker_init(neuworker_t *nw, void *data){
    ASSERT(nw != NULL);

    nw->base = event_base_new();
    if(!nw->base)
        return -1;    

    if(!evutil_socketpair(AF_LOCAL, SOCK_STREAM, 0, nw->socks)){
        event_base_free(nw->base);
        return -1;
    }
    evutil_make_socket_nonblocking(nw->socks[0]);
    evutil_make_socket_nonblocking(nw->socks[1]);

    nw->evsock = event_new(nw->base, nw->socks[0], EV_READ, neuworker_sockcb, nw);
    if(nw->evsock == NULL){
        event_base_free(nw->base);
        evutil_closesocket(nw->socks[0]);
        evutil_closesocket(nw->socks[1]);
        return -1;
    }
    event_add(nw->evsock, NULL);

    pthread_mutex_init(&nw->lock, NULL);

    nw->data = data;

    return 0;
}

int neuworker_fini(neuworker_t *nw){
    ASSERT(nw != NULL);

    if(!nw->base)
        return 0;
    
    event_base_loopbreak(nw->base);
    event_base_free(nw->base);
    nw->base = NULL;

    evutil_closesocket(nw->socks[0]);
    evutil_closesocket(nw->socks[1]);

    pthread_mutex_destroy(&nw->lock);

    return 0;
}

int neuworker_dispatch(neuworker_t *nw, neuhandle_t *nh){
    int ret;

    ASSERT((nw != NULL) && (nh != NULL));

    pthread_mutex_lock(&nw->lock);
    ret = write(nw->socks[1], nh, sizeof(*nh));
    pthread_mutex_unlock(&nw->lock);

    return (ret == sizeof(*nh)) ? 0 : -1;
}

static void neuservice_accept_cb(struct evconnlistener *evl, evutil_socket_t sock,
            struct sockaddr *addr, int socklen len, void *data){
    neuservice_t *ns = (neuservice_t *)data;
    neusession_t *ses;

    ses = neusession_new(ns, sock);
    if(ses == NULL){
        close(sock);
        return ;
    }

    if(ns->on_setup != NULL){
        ns->on_setup(ns, ses, ns->ns_data);
    }else{
        neusession_free(ns);
    }
}

static void neuservice_shutdown_cb(struct evconnlistener evl*, void *data){
    neuservice_t *ns = (neuservice_t *)data;

    if(ns->on_shutdown)
        ns->on_shutdown(ns, ns->ns_data);
}

static void watch_service_cb(neuworker_t *nw, struct neuhandle *nh, void *data){
    struct neuservice *ns = (sstruct neuservice *)data;
    
    ASSERT((nw != NULL) && (ns != NULL));

    ns->ns_listen = evconnlistener_new_bind(nw->base, neuservice_accept_cb,
        ns, 0, -1, (struct sockaddr*)&ns->ns_addr, sizeof(ns->ns_addr));

    evconnlistener_set_error_cb(ns->ns_listen, neuservice_shutdown_cb);
}

int neuworker_watch_service(neuworker_t *nw, struct neuservice *ns){
    neuhandle_t nh;
    int ret;

    ASSERT((nw != NULL) && (ns != NULL));

    nh.callback = watch_service_cb;
    nh.data = ns;

    pthread_mutex_lock(&nw->lock);
    ret = write(nw->socks[1], &nh, sizeof(nh));
    pthread_mutex_unlock(&nw->lock);

    return (ret == sizeof(nh)) ? 0 : -1;
}

static void watch_session_cb(neuworker_t *nw, struct neuhandle *nh, void *data){
    struct neusession *ses = (struct neusession *)data;

    ASSERT((nw != NULL) && (nh != NULL));

    ses->ev = event_new(nw->base, ses->sock, EV_READ|EV_WRITE,
                        neusession_event_cb, ses);
    
    if(ses->ev == NULL){
        ses->ns->on_close(ses->ns, ses, ses->data);
        neusession_free(ses);
        return -1;
    }

    return event_add(ses->ev, NULL);
}

int neuworker_watch_session(neuworker_t *nw, struct neusession *ns){
    neuhandle_t nh;
    int ret;

    ASSERT((nw != NULL) && (ns != NULL));

    nh.callback = watch_session_cb;
    nh.data = ns;

    pthread_mutex_lock(&nw->lock);
    ret = write(nw->socks[1], &nh, sizeof(nh));
    pthread_mutex_unlock(&nw->lock);

    return (ret == sizeof(nh)) ? 0 : -1;
}

