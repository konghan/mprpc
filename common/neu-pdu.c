#include "neu-pdu.h"

#include "neuron.h"

#include <errno.h>

#define NEUPDU_SLICE_MAX        (1024*16)

neupdu_t *neupdu_chunk_new(neusession_t *ses, int size, int slice, int header){
    neupdu_t *pdu;
    int num, len, i;
    
    ASSERT(ses != NULL);

    if(header != 0){
        num = ((size - header) + slice -1) / slice;
    }else{
        num = (size + slice - 1) / slice;
    }
    pdu = (neupdu_t*)xmalloc(sizeof(*pdu) + sizeof(struct iovec)*num);
    if(pdu == NULL){
        return NULL;
    }

    if(header != 0){
        pdu->iovs[0].iov_len = header;
        pdu->iovs[0].iov_base = xmalloc(header);
        if(pdu->iovs[0].iov_base == NULL){
            xfree(pdu);
            return NULL;
        }
        len = size - header;
        i = 1;
    }else{
        len = size;
        i = 0;
    }
    
    for(; i < num; i++){
        pdu->iovs[i].iov_len = len > slice ? slice : len;
        pdu->iovs[i].iov_base = xmalloc(pdu->iovs[i].iov_len);
        if(pdu->iovs[i].iov_base == NULL)
            goto err_out;

        len -= slice;
    }
    
    pdu->header = (neupdu_header_t*)pdu->iovs[0].iov_base;
    pdu->num = num;
    pdu->ses = ses;
    pdu->start = pdu->end = 0;

    return pdu;

err_out:

   for(i -= 1; i >= 0; i--){
        if(pdu->iovs[i].iov_base != NULL){
            xfree(pdu->iovs[i].iov_base);
        }
    }

    return NULL;
}

neupdu_t *neupdu_new(neusession_t *ses, int size){
    neupdu_t *p;

    p = (neupdu_t*)xmalloc(sizeof(*p) + size);
    if(p == NULL){
        return NULL;
    }
    p->header = (neupdu_header_t*)p->payload;
    p->ses = ses;
    p->num = 0;
    p->start = p->end = 0;
    
    return p;
}

void neupdu_free(neupdu_t *pdu){
    int i;

    if(pdu->num != 0){
        for(i = 0; i < pdu->num; i++){
            if(pdu->iovs[i].iov_base != NULL){
                xfree(pdu->iovs[i].iov_base);
            }
        }
    }

    xfree(pdu);
}

void neupdu_header_ntoh(neupdu_header_t *header){
    header->len = (uint32_t)ntohl(header->len);
    header->cmd = (uint16_t)ntohs(header->cmd);
    header->seq = (uint32_t)ntohl(header->seq);
}

void neupdu_header_hton(neupdu_header_t *header){
    header->len = (uint32_t)htonl(header->len);
    header->cmd = (uint16_t)htons(header->cmd);
    header->seq = (uint32_t)htonl(header->seq);
}

int neupdu_recv(neusession_t *ses, neupdu_t **pdu){
    neupdu_header_t header;
    neupdu_t    *p;
    int ret;

    ASSERT((ses != NULL) && (pdu != NULL));

    ret = recv(ses->sock, &header, sizeof(header), MSG_PEEK);
    if(ret != sizeof(header)){
        neulog_warn("data less than header in socket buffer!\n");
        return ret;
    }
    neupdu_header_ntoh(&header);
    
    if(NEUPDU_GET_TYPE(header.cmd) == NEUPDU_TYPE_CHUNK){
        //FIXME : chunk mode 
        return -1;
    }else{
        if(header.len <= NEUPDU_SLICE_MAX){
            p = neupdu_new(ses, header.len);
            if(p == NULL){
                return -ENOMEM;
            }

            ret = recv(ses->sock, p->payload, header.len, 0);
            if(ret != header.len){
                neupdu_free(p);
                return ret;
            }
            p->end = ret;

        }else{
            p = neupdu_chunk_new(ses, header.len, NEUPDU_SLICE_MAX, 0);
            if(p == NULL){
                return -ENOMEM;
            }
            
            ret = readv(ses->sock, p->iovs, p->num);
            if(ret != header.len){
                neupdu_free(p);
                return -1;
            }

            p->end = ret;  
        }
    }
    
    neupdu_header_ntoh(p->header);
    *pdu = p;

    return 0;
}

int neupdu_read(neupdu_t *pdu, void *buf, int len){
    return 0;
}

int neupdu_write(neupdu_t *pdu, const void *buf, int len){
    return 0;
}

