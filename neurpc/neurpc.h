

#ifndef __SVRPC_H__
#define __SVRPC_H__

#ifdef __cpluscplus
extern "C" {
#endif

struct svrpc;
typedef struct svrpc_t;

typedef int (*svrpc_cbfunc)(svrpc_t *m, const void *req, void **rsp);

struct svrpc_callback{
    int mc_idx;
    svrpc_cbfunc mc_func;
};

struct svrpc_service{
    int ms_num;
    struct svrpc_callback ms_cbs[];
};
typedef struct svrpc_service svrpc_service_t;

extern int svrpc_create(svrpc_t **m);
extern int svrpc_destroy(svrpc_t *m);

extern int svrpc_loop(svrpc_t *m);

extern int svrpc_add(svrpc_t *m, const char *name, svrpc_service_t *svc);
extern int svrpc_del(svrpc_t *m, const char *name, svrpc_service_t **svc);

/*
 *
 */

struct svrpc_clt;
typedef struct svrpc_clt svrpc_clt_t;
extern int svrpc_clt_init(


#ifdef __cpluscplus
}
#endif

#endif // __SVRPC_H__

