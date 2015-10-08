
#include "neu-session.h"

#include "neuron.h"

#include <string.h>

neusession_t *neusession_new(struct neuservice *ns, evutil_socket_t sock){
    neusession_t *ses;

    ASSERT(ns != NULL);
    
    ses = xmalloc(sizeof(*ses));
    if(ses == NULL)
        return NULL;
    memset(ses, 0, sizeof(*ses));
    
    ses->ns = ns;
    ses->sock = sock;

    return ses;
}

void neusession_free(neusession_t *ses){
    xfree(ses);
}

static void neusession_close(neusession_t *ses, int errcode){
}

void neusession_event_cb(evutil_socket_t sock, short flags, void *data){
    neusession_t *ses = (neusession_t *)data;
    neuservice_t *srv = (neuservice_t *)ses->ns;
    neupdu_t *pdu;
    neumsg_t *nm;
    int slot;
    int ret;

    if(flags & EV_READ){
        while(1){
            ret = neupdu_recv(ses, &pdu);
            if(ret < 0){
                neusession_close(ses, NEUERROR_PDUERR);
                return ;
            }

            if(ret > 0){
                ses->pdu = pdu;
                neulog_info("PDU not complete\n");
                break;
            }
            
            slot = NEUPDU_GET_ID(pdu->header->cmd);
            switch(NEUPDU_GET_TYPE(pdu->header->cmd)){
            case NEUPDU_TYPE_CHUNK:
                break;

            case NEUPDU_TYPE_MSG:
                if(fixarray_get(&srv->ns_msgs, slot, (void**)&nm) != 0){
                    nm->dispatch(ses, pdu);
                }else{
                    neusession_close(ses, NEUERROR_MSGERR);
                    neupdu_free(pdu);
                }
                break;
            
            case NEUPDU_TYPE_RPC:
                break;

            case NEUPDU_TYPE_MSQ:            
                break;
            
            default:
                break;
            }
        }
    }

    if(flags & EV_WRITE){
        // send pending pdus
    }

//    if(flags & EV_ERROR){
//    }

}

