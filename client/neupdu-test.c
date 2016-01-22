
#include "neupdu.h"

#include "logger.h"

#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#define SLICE_SIZE          4096

int neupdu_test_newfree(){
    neupdu_t *chunk;
    neupdu_t *pdu;

    log_info("================\n");
    log_info("alloc pdu in chunk mode, only meta\n");
    chunk = neupdu_chunk_new(256*1024, SLICE_SIZE, 0);
    if(chunk == NULL){
        log_warn("alloc chunk fail!\n");
        return -1;
    }
    log_info("alloc chunk without data buffer ok\n");
    neupdu_free(chunk);

    chunk = neupdu_chunk_new(128*1024, SLICE_SIZE, 1);
    if(chunk == NULL){
        log_warn("alloc chunk fail: prepare data buffer\n");
        return -1;
    }
    log_info("alloc chunk with data buffer ok!\n");
    neupdu_free(chunk);

    log_info("alloc pdu in payload mode\n");
    pdu = neupdu_new(16*1024);
    if(pdu == NULL){
        log_warn("alloc pdu fail!\n");
        return -1;
    }
    log_info("alloc pdu test ok!\n");
    neupdu_free(pdu);

    return 0;
}

int neupdu_test_readwrite(){
    neupdu_t *pdu;
    int chunk = 0;
    int ret;
    char bufin[128];
    char bufout[128];

    log_info("================\n");
   
    log_info("test payload buffer mode\n");
    pdu = neupdu_new(32);
    if(pdu == NULL){
        log_warn("alloc pdu fail\n");
        return -1;
    }

try_again:

    sprintf(bufin, "0123456789012345678901234567890123456789\n");

    neupdu_write(pdu, bufin, 31);
    neupdu_read(pdu, bufout, 31);
    bufout[31] = 0;
    log_info("read : %s\n", bufout);

    neupdu_reset(pdu);

    neupdu_write(pdu, bufin, 32);
    neupdu_read(pdu, bufout, 32);
    bufout[32] = 0;
    log_info("read : %s\n", bufout);

    neupdu_reset(pdu);

    ret = neupdu_write(pdu, bufin, 33);
    if(ret == 32){
        log_info("only buffer size write ok\n");
    }else{
        log_warn("pdu write overflow : %d\n", ret);
        return -1;
    }
    neupdu_free(pdu);

    switch(chunk){
    case 0:
        pdu = neupdu_chunk_new(32, 16, 0);
        if(pdu == NULL){
            log_warn("alloc chunk fail!\n");
            return -1;
        }
        chunk = 1;

        log_info("test chunk without buffer mode\n");
        goto try_again;
        break;
        
    case 1:
        pdu = neupdu_chunk_new(32, 16, 1);
        if(pdu == NULL){
            log_warn("alloc chunk fail!\n");
            return -1;
        }
        chunk = 2;
        
        log_info("test chunk with buffer mode\n");
        goto try_again;
        break;

    case 2:
        break;
    }

    return 0;
}

int neupdu_test_sendrecv(){
    neupdu_t *chunkin, *chunkout;
    int in, out;
    char bufin[64], bufout[128];
    int sock[2];

    log_info("================\n\n");

    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sock) != 0){
        log_warn("socketpair fail!\n");
        return -1;
    }

    chunkin = neupdu_chunk_new(128, 32, 1);
    if(chunkin == NULL){
        log_warn("alloc chunk fail!\n");
        return -1;
    }
    
    strcpy(bufin, "test for neupdu!\n");
    in = strlen(bufin);

    neupdu_write(chunkin, &in, sizeof(in));
    neupdu_write(chunkin, bufin, strlen(bufin));

    neupdu_send(sock[0], chunkin);

    neupdu_recv(sock[1], &chunkout);

    neupdu_read(chunkout, &out, sizeof(out));
    neupdu_read(chunkout, bufout, out);

    neupdu_free(chunkin);
    neupdu_free(chunkout);

    if(in != out){
        log_warn("neupdu send recv fail: %d %d\n", in, out);
        return -1;
    }

    log_info("neupdu send recv ok!\n");

    return 0; 
}

int main(){

    logger_init(NULL, "127.0.0.1", 4040, 0);

    neupdu_test_newfree();

    neupdu_test_readwrite();

    neupdu_test_sendrecv();    

    return 0;
}

