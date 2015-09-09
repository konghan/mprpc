

#ifndef __TEMPLATE_GEN_H__
#define __TEMPLATE_GEN_H__

#ifdef __cpluscplus
extern "C" {
#endif


struct template_service{
    int rpc_func00(void *inst, int param0, void *param1, char *param2);
    int rpc_func01(void *inst, void *param1, char *param2);
    int rpc_func02(void *inst, int param0, char *param2);
    
};


extern int template_accepted_callback(void *inst, neurpc_session_t *ses);
extern int template_closed_callback(void *inst, neurpc_session_t *ses);

extern int template_rpc_func00(void *inst, int param0, void *param1, char *param2);
extern int template_rpc_func01(void *inst, void *param1, char *param2);
extern int template_rpc_func02(void *inst, int param0, char *param2);

#ifdef __cpluscplus
}
#endif

#endif // __TEMPLATE_GEN_H__

