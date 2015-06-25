
#include "neusvr.h"

#include <stdlib.h>
#include <assert.h>


static void neusvr_on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf){
    neucli_t *cli = container_of(stream, neucli_t);

    if(cli
}

static void neusvr_on_connect(uv_stream_t* req, int status){
    neucli_t *cli;

    if(status < 0){
        return ;
    }

    cli = malloc(sizeof(neucli_t));
    if(cli == NULL){
        return ;
    }

    uv_tcp_init(req->loop, &cli->nc_cli);
    
    if(uv_accept(req, (uv_stream_t *)&cli->nc_cli) == 0){
        // add to neusvr container
        uv_read_start((uv_stream_t)&cli->nc_cli, alloc_buffer, neusvr_on_read);
    }else{
        free(cli);
    }
}

int neusvr_init(neusvr_t *ns, struct sockaddr_in *addr){
    int ret;

    assert((ns != NULL) && (addr != NULL));

    ret = uv_loop_init(&ns->ns_loop);
    if(ret != 0){
        return ret;
    }

    ret = uv_tcp_bind(&ns->ns_server, (const struct sockaddr*)addr, 0);
    if(ret != 0){
        goto err_bind;
    }

    ret = uv_listen((uv_stream_t *)&ns->ns_server, 0, neusvr_on_connect);
    if(ret != 0){
        goto err_listen;
    }

    return 0;

err_listen:

err_bind:
    uv_loop_close(&ns->ns_loop);

    return ret;
}

void neusvr_destroy(neusvr_t *ns){
}

void neusvr_loop(neusvr_t *ns){
}

int neusvr_add_svrpc(neusvr_t *ns, const char *name, svrpc_t *svrpc);
int neusvr_rmv_svrpc(neusvr_t *ns, int idx);


