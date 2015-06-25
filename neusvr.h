

#ifndef __NEUSVR_H__
#define __NEUSVR_H__

#include <uv.h>

#ifdef __cpluscplus
extern "C" {
#endif

struct neusvr {
    uv_loop_t ns_loop;
    uv_tcp_t  ns_server;
};

typedef struct neusvr neusvr_t;

int neusvr_init(neusvr_t *ns, struct sockaddr_in *addr);
void neusvr_fini(neusvr_t *ns);

void neusvr_loop(neusvr_t *ns);

int neusvr_add_svrpc(neusvr_t *ns, const char *name, svrpc_t *svrpc);
int neusvr_rmv_svrpc(neusvr_t *ns, int idx);

struct neucli {
    uv_tcp_t nc_cli;
};
typedef struct neucli neucli_t;


#ifdef __cpluscplus
}
#endif

#endif // __NEUSVR_H__

