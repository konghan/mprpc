

#include "neu-service.h"

#include "neuron.h"

#include <string.h>

#define DEFAULT_SERVICE_NUM         64

int neuservice_init(neuservice_t *ns, struct sockaddr_in *addr, 
        neuservice_on_setup_session setupcb,
        neuservice_on_close_session closecb, 
        neuservice_on_shutdown shutdowncb, void*data){

    ASSERT((ns != NULL) && (addr != NULL));

    memcpy(&ns->ns_addr, addr, sizeof(*addr));

    ns->on_setup = setupcb;
    ns->on_close = closecb;
    ns->on_shutdown = shutdowncb;
    ns->ns_data = data;

    fixarray_init(&ns->ns_msgs, DEFAULT_SERVICE_NUM);
    fixarray_init(&ns->ns_rpcs, DEFAULT_SERVICE_NUM);
    fixarray_init(&ns->ns_msqs,  DEFAULT_SERVICE_NUM);

    return 0;
}

int neuservice_fini(neuservice_t *ns){
    return -1;
}


int neuservice_regist_msg(neuservice_t *ns, neumsg_t *msg){
    ASSERT((ns != NULL) && (msg != NULL));
    
    return fixarray_append(&ns->ns_msgs, msg);
}

/*
int neuservice_regist_rpc(neuservice_t *ns, neurpc_t *rpc){
    ASSERT((ns != NULL) && (rpc != NULL));

    return fixarray_append(&ns->ns_rpcs, rpc);
}

int neuservice_regist_topic(neuservice_t *ns, neutopic_t *topic){
    ASSERT((ns != NULL) && (topic != NULL));

    return fixarray_append(&ns->ns_topic, topic);
}
*/

