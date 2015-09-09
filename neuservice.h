

#ifndef __NEUSERVICE_H__
#define __NEUSERVICE_H__

#include "neuloop.h"

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct neuservice neuservice_t;

int neuservice_init(neuservice_t *ns, struct sockaddr_in *addr);
int neuservice_fini(neuservice_t *ns);

int neuservice_loop(neuloop_t *nl, neuservice_t *ns);

int neuservice_regist_rpc(neuservice_t *ns, neurpc_t *rpc);
int neuservice_add_neutopic(neuservice_t *ns, neutopic_t *topic);

#ifdef __cpluscplus
}
#endif

#endif // __NEUSERVICE_H__

