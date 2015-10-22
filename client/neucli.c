
#include "neucli.h"

#include <event2/util.h>

#include <pthread.h>
#include <semaphore.h>
#include <err.h>



struct neusrv{
    // public 
    void *data;

    // private
    pthread_mutex_t lock;
    evutil_socket_t socks[2];  // for management, 0-w;1-r

    struct event_base *base;

    pthread_t   thrd; // worker thread
};

enum {
    NEUSES_STATE_INIT = 0,
    NEUSES_STATE_NEGO,      // sync data between client & server
    NEUSES_STATE_NORM,      // normal state
    NEUSES_STATE_CLOSE
};

struct neuses{
    // public 
    void *data;

    // private
    evutil_socket_t sock;

    struct neusrv *srv;
    event_t *evconn;

    int state;  // state machine

    sem_t *sem;
};

struct neurpc;
struct neumsg;
struct neumsq;

enum {
    NEUCMD_SES_SETUP = 0,
    NEUCMD_SES_CLOSE,
    NEUCMD_MSG_SEND,
    NEUCMD_RPC_CALL,
    NEUCMD_MSQ_SUBSCRIBE,
    NEUCMD_MSG_UNSUBSCRIBE,
    NEUCMD_STATS,
};

struct neucmd {
    int cmd;
    void *data;
};

////////////////////

static void session_proc(evutil_socket_t sock, short type, void *data){
    struct neuses *ses = (struct neuses *)data;

    if(type & EV_READ){
        switch(ses->state){
        case NEUSES_STATE_INIT:
            //
            break;

        case NEUSES_STATE_NEGO:
           // TODO: read server info
            ses->state = NEUSES_STATE_NORM;
            sem_post(ses->sem);
            break;

        case NEUSES_STATE_NORM:
            // TODO: dispatch respone to callback
            break;

        case NEUSES_STATE_CLOSE:
            break;

        default:
            break;
        }
    }

    if(type & EV_WRITE){
        switch(ses->state){
        case NEUSES_STATE_INIT:
            ses->state = NEUSES_STATE_NEGO;
            // TODO: request server info
            break;

        case NEUSES_STATE_NEGO:
            // ERROR
            break;

        case NEUSES_STATE_NORM:
            break;
        }
        }
    }
}


static void command_proc(evutil_socket_t sock, short type, void *data){
    struct neusrv *srv = (struct neusrv*)data;
    struct neuses *ses;
    struct neucmd  cmd;
    int nr;

    if(type & EV_READ){
        nr = read(sock, &cmd, sizeof(cmd));
        if(nr != sizeof(cmd)){
            // fatal
            return;
        }

        switch(cmd.cmd){
        case NEUCMD_SETUP_SES:
            ses = (struct neuses *)cmd.data;
            ses->evconn = event_new(srv->base, ses->sock, EV_READ|EV_WRITE, session_proc, ses);
            event_add(ses->evconn, NULL);
            break;

        case NEUCMD_CLOSE_SES:
            break;
        }
    }
}

static void *worker_routine(void *arg){
    struct neusrv *srv = (struct neusrv*)arg;

    event_base_dispatch(srv->base);

    return NULL;
}

int neusrv_init(struct neusrv *srv, void *data){
    srv->data = data;

    pthread_mutex_init(&srv->lock, NULL);

    if(!evutil_socketpair(AF_LOCAL, SOCK_STREAM, 0, srv->socks)){
        return -1;
    }
    
    srv->base = event_base_new();
    if(srv->base == NULL){
        return -1;
    }

    srv->evcmd = event_new(srv->base, srv->socks[1], EV_READ, command_proc, srv);
    event_add(srv->evcmd, NULL);

    return pthread_create(&srv->thrd, NULL, worker_routine, srv);
}

int neusrv_fini(struct neusrv *srv){
    return 0;
}

static int neusrv_dispatch(struct neusrv *srv, int cmd, void *data){
    struct neucmd cmd;

    cmd.cmd = NEUCMD_SETUP_SES;
    cmd.data = ses;

    pthread_mutex_lock(&srv->lock);
    send(srv->socks[0], &cmd, sizeof(cmd), 0);
    pthread_mutex_unlock(&srv->lock);

    return 0;
}

int neusrv_setup_ses(struct neusrv *srv, struct sockaddr *addr, int len, struct neuses **ses){
    struct neuses *ses;
    struct neucmd cmd;

    ses = (struct neuses*)xmalloc(sizeof(*ses));
    if(ses == NULL){
        return -1;
    }

    ses->sock = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(ses->sock, addr, len) != 0){
        xfree(ses);
        return -1;
    }

    ses->sem = sem_open(NULL, O_CREAT, 0777, 0);
    if(ses->sem == NULL){
        closesocket(ses->sock);
        xfree(ses);
        return -1;
    }

    neusrv_dispatch(srv, NEUCMD_SETUP_SES, ses);

    sem_wait(ses->sem);
    ses->srv = srv;

    return 0;
}

int neusrv_close_ses(struct neuses *ses){

    neusrv_dispatch(srv, NEUCMD_CLOSE_SES, ses);

    sem_wait(ses->sem);

    closesockeet(ses->sock);

    sem_destroy(ses->sem);

    xfree(ses);

    return 0;
}


int neuses_setup_msg(struct neuses *ses, const char *msg, struct neumsgcli **mcli){
    struct neumsgcli *mc;

    mc = (struct neumsgcli *)xmalloc(sizeof(*mc));
    if(mc == NULL){
        return -ENOMEM;
    }

    // find msg name
    // set mc msg id;

    *mcli = mc;

    return 0;
}

int neuses_close_msg(struct neumsgcli *mcli);

typedef void (*neumsg_rspcb)(void *data, uint32_t seq, struct neupdu *rsp);
// message name send()
int neumsg_sendex(struct neumsgcli *mcli, struct neupdu *req,
                  neumsg_rspcb cb, void *data){
    return 0;
}

int neumsg_send(struct neumsgcli *mcli, struct neupdu *req){
    return neumsg_sendex(mcli, req, NULL, NULL);
}

/** message queue
*/

//int neusrv_setup_msq(struct neusrv *srv, const char *msq, struct meumsqcli *mcli);
//int neusrv_close_msq(struct neumsqcli *mcli);

//typedef void (*neumsq_cb)(struct neumsqcli *mcli, void *data, struct neupdu *msg);
//int neusrc_subscribe(struct neumsqcli *mcli, const char *msq, neumsq_cb cb, void *data);
//int neusrc_unsubscribe(struct neumsqcli *mcli);


/** RPC
*/

//int neusrv_setup_rpc(struct neusrv *srv, const char *service, struct neurpc **rpc);
//int neusrv_close_rpc(struct neurpc *rpc);

//typedef void (*neurpc_cb)(struct neurpc *rpc, unint32_t seq, void *data, struct neupdu *rsp);
//int neurpc_call(struct neurpc *rpc, int func, struct neupdu *req, struct neupdu **rsp);
//uint32_t neurpc_call_async(struct neurpc *rpc, int func, struct neupdu *req,
//            neurpc_cb cb, void *data);

