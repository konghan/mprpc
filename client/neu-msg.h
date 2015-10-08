

#ifndef __NEU_MSG_H__
#define __NEU_MSG_H__

#ifdef __cpluscplus
extern "C" {
#endif

enum {
    NEUMSG_TYPE_IOV   = 0,
    NEUMSG_TYPE_BUF,
};

struct neupdu;

typedef void (*neumsg_dispatch)(struct neusession *ses, struct neupdu *pdu);

typedef struct neumsg{
    int type;       // iov or buffer
    int header;     // header size
    int slice;      // slice size in iov mode

    neumsg_dispatch dispatch;
}neumsg_t;

#ifdef __cpluscplus
}
#endif

#endif // __NEU_MSG_H__

