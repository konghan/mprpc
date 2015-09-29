
#ifndef __NEU_SESSION_H__
#define __NEU_SESSION_H__

#include <event2/event.h>

#ifdef __cpluscplus
extern "C" {
#endif

// typedef static int (*neusession_dispatch)(nueses_t *ses, neupdu_t *req, neupdu_t **rsp);

struct neuservice;
struct neupdu;

typedef struct neusession{
    // public data of this session   
    void *data;

    struct neupdu *pdu;

    // private data of this session

    // neuservice which contain this session
    struct neuservice *ns;

    struct event *ev;
    int sock;

    // session info
    uint32_t seq;
    int window;

    // struct list_header ns_handles;

    // statistic
}neusession_t;

neusession_t *neusession_new(struct neuservice *ns, evutil_socket_t sock);
void neusession_free(neusession_t *ses);

static inline void neusession_set_data(neusession_t *ns,  void *data){
    if(ns != NULL)    
        ns->data = data;
}

static inline void *neusession_get_data(neusession_t *ns){
    if(ns != NULL)
        return ns->data;

    return NULL;
}

// internal used only!
void neusession_event_cb(evutil_socket_t sock, short flags, void *data);

#ifdef __cpluscplus
}
#endif

#endif // __NEU_SESSION_H__

