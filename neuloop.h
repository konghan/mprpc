

#ifndef __NEULOOP_H__
#define __NEULOOP_H__

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct neuloop neuloop_t;
typedef struct neuhandle neuhandle_t;

int neuloop_init(neuloop_t *nl);
int neuloop_fini(neuloop_t *nl);

void neuloop_run(neuloop_t *nl);

int neuloop_dispatch_workload(neuloop_t *nl, neuhandle_t *nh);

#ifdef __cpluscplus
}
#endif

#endif // __NEULOOP_H__

