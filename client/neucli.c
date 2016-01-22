
#include "neucli.h"

#include <event2/util.h>

#include <pthread.h>
#include <semaphore.h>
#include <err.h>

static void neucli_onevent(evutil_socket_t efd, short flags, void *data){
    neucli_t *cli = (neucli_t *)data;

    switch(flags){
    case EV_WTRITE:
        switch(cli->state){
        case NEUCLI_CONN_INIT:
            cli->state = NEUCLI_CONN_CONNECTED;
            cli->onconn(cli, cli->state, 0);
            // FIXME : request meta data

            break;

        case NEUCLI_CONN_CONNECTED:
            break;
        }
        break;
    
    case EV_READ:
        switch(cli->state){
        case NEUCLI_CONN_INIT:
            break;

        case NEUCLI_CONN_CONNECTED:
            // FIXME : parse meta data

            cli->state = NEUCLI_CONN_SYNCED;
            cli->onconn(cli, cli->state, 0);
            break;

        case NEUCLI_CONN_SYNCED:
            cli->ondata(cli);
            break;
        }
        break;
    
    default:
        // FIXME : connection broken, drop it!

        cli->onconn(cli, cli->state, -1);
        break;
    }
}

int neucli_init(neucli_t *cli, neucli_on_connect onconn, neucli_on_data ondata){
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    cli->efd = socket(AF_INET, SOCK_STREAM, 0);
    cli->state = NEUCLI_CONN_INIT;

    cli->onconn = onconn;
    cli->ondata = ondata;

    NEUEVENT_INIT(&cli->nev, cli, neucli_onevent, cli->efd, 0, tv);

    return 0;
}

int neucli_fini(neucli_t *cli){

    cli->state = NEUCLI_CONN_INIT;

    return 0;
}

int neucli_connect(neucli_t *cli, struct sockaddr *addr, int len){
    int ret;

    if(cli->state != NEUCLI_CONN_INIT){
        return -1;
    }

    ret =  connect(cli->efd, addr, len);
    if(ret != 0){
        return ret;
    }

    return neuworker_watch_event(cli->nw, &cli->nev);
}

static inline int neucli_send_pdu(neucli_t *cli, neupdu_t *pdu){
    return 0;
}

int neucli_send(neucli_t *cli, neupdu_t *pdu){
    int ret;

    if(cli->state != NEUCLI_CONN_SYNCED){
        return -1;
    }

    return neucli_send_pdu(cli, pdu);
}


