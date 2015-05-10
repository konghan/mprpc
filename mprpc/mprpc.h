

#ifndef __MPRPC_H__
#define __MPRPC_H__

#ifdef __cpluscplus
extern "C" {
#endif

struct mprpc;
typedef struct mprpc_t;

typedef int (*mprpc_cbfunc)(mprpc_t *m, const void *req, void **rsp);

struct mprpc_callback{
    int mc_idx;
    mprpc_cbfunc mc_func;
};

struct mprpc_service{
    int ms_num;
    struct mprpc_callback ms_cbs[];
};
typedef struct mprpc_service mprpc_service_t;

extern int mprpc_create(mprpc_t **m);
extern int mprpc_destroy(mprpc_t *m);

extern int mprpc_loop(mprpc_t *m);

extern int mprpc_add(mprpc_t *m, const char *name, mprpc_service_t *svc);
extern int mprpc_del(mprpc_t *m, const char *name, mprpc_service_t **svc);

/*
**
*/

struct mprpc_clt;
typedef struct mprpc_clt mprpc_clt_t;
extern int mprpc_clt_init(


#ifdef __cpluscplus
}
#endif

#endif // __MPRPC_H__

