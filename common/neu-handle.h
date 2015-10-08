

#ifndef __NEU_HANDLE_H__
#define __NEU_HANDLE_H__

#ifdef __cpluscplus
extern "C" {
#endif

struct neuhandle;
struct neuworker;
typedef void (*neuhandle_callback)(struct neuworker *nw, struct neuhandle *nh, void *data);

struct neuhandle{
    // public
    void *data;
    neuhandle_callback  callback;
};
typedef struct neuhandle neuhandle_t;

static inline void neuhandle_init(neuhandle_t *nh, neuhandle_callback cb, void *data){
    if(nh != NULL){
        nh->callback = cb;
        nh->data = data;
    }
}

static inline void neuhandle_call(struct neuworker *nw, neuhandle_t *nh){
    if((nh != NULL) && (nh->callback != NULL)){
        nh->callback(nw, nh, nh->data);
    }
}

#ifdef __cpluscplus
}
#endif

#endif // __NEU_HANDLE_H__

