
#include "neuron.h"

static void on_setup(neuservice_t *ns, neusession_t *ses, void *data){
    printf("==>session setup\n");

    neuworker_watch_session(ns->ns_worker, ses);
}

static void on_close(neuservice_t *ns, neusession_t *ses, void *data){
    printf("==>session closed!\n");
}

static void on_shutdown(neuservice_t *ns, void *data){
    printf("==>service shutdown\n");
}

static void msg_dispatch(struct neusession *ses, struct neupdu *pdu){
    printf("==>recv pdu %d\n", (int)pdu);
}

int main(){
    neuworker_t nw;
    neuservice_t ns;
    neumsg_t nm;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    neuworker_init(&nw, NULL);

    neuservice_init(&ns, &addr, on_setup, on_close, on_shutdown, NULL);

    nm.type = NEUMSG_TYPE_BUF;
    nm.header = 0;
    nm.slice = 0;
    nm.dispatch = msg_dispatch;
    neuservice_regist_msg(&ns, &nm);

    neuworker_watch_service(&nw, &ns);

    neuworker_run(&nw);

    return 0;
}
