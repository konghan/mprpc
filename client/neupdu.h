

#ifndef __NEUPDU_H__
#define __NEUPDU_H__

#include "osapi.h"

#include <stdint.h>
#include <sys/uio.h>

#ifdef __cpluscplus
extern "C" {
#endif

// cmd - 16bits{|-1-|-3-|-12-|, -1- reqrsp, -3- type, -12- id }
#define NEUPDU_REQRSP           0x8000
#define NEUPDU_TYPE             0x7000
#define NEUPDU_ID               0x0FFF

enum {
    NEUPDU_TYPE_CHUNK = 0,
    NEUPDU_TYPE_MSG,
    NEUPDU_TYPE_RPC,
    NEUPDU_TYPE_MSQ,
};

#define NEUPDU_IS_REQ(cmd)      ((cmd) & NEUPDU_REQRSP)
#define NEUPDU_SET_REQ(cmd)     ((cmd) |=  NEUPDU_REQRSP)
#define NEUPDU_SET_RSP(cmd)     ((cmd) &= ~NEUPDU_REQRSP)

#define NEUPDU_GET_TYPE(cmd)            (((cmd) & NEUPDU_TYPE) >> 12)
#define NEUPDU_SET_TYPE(cmd,pdutype)    ((cmd) |= ((pdutype  << 12) & NEUPDU_TYPE))

#define NEUPDU_GET_ID(cmd)              ((cmd) & NEUPDU_ID)
#define NEUPDU_SET_ID(cmd,pduid)        ((cmd) |= pduid & NEUPDU_ID)

typedef struct neupdu_header{
    uint32_t len;  // size of this pdu
    uint16_t cmd;  // command of this pdu
    uint8_t  err;  // response code
    uint8_t  num;  // number of message
    uint32_t seq;  // sequence number
}__attribute__((packed))neupdu_header_t;

typedef struct neupdu{
    // public data
    void *data;

    // private data
    int start;
    int start_offset;

    int end;
    int end_offset;

    int num;   // num of iov, zero for chunk mode

    // in iov mode, iovs[0] always for header    

    neupdu_header_t header;
    union{
        struct iovec    iovs[0];
        char payload[0];
    };
}neupdu_t;

// size : total size of buffer;
// slice : iov data size;
neupdu_t *neupdu_chunk_new(int size, int slice, int alloc_mem);
neupdu_t *neupdu_new(int size);

void neupdu_free(neupdu_t *pdu);

int neupdu_read(neupdu_t *pdu, void *buf, int len);
int neupdu_write(neupdu_t *pdu, const void *buf, int len);

int neupdu_send(int sock, neupdu_t *pdu);
int neupdu_recv(int sock, neupdu_t **pdu);

static inline void neupdu_reset(neupdu_t *pdu){
    if(pdu->num){
        pdu->start = 1;
        pdu->start_offset = 0;
        pdu->end = 1;
        pdu->end_offset = 0;
    }else{
        pdu->start = 0;
        pdu->start_offset = 0;
        pdu->end_offset = 0;
    }
}

#ifdef __cpluscplus
}
#endif

#endif // __NEUPDU_H__

