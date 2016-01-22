
#include "neupdu.h"

#include "logger.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NEUPDU_SLICE_MAX        (1024*16)

neupdu_t *neupdu_chunk_new(int size, int slice, int alloc_mem){
    neupdu_t *pdu;
    int num, len, i;
    
    num = (size + slice - 1) / slice + 1; // 1 - for header
    len = sizeof(*pdu) + sizeof(struct iovec)*num;
    
    pdu = (neupdu_t*)xmalloc(len);
    if(pdu == NULL){
        return NULL;
    }
    memset(pdu, 0, len);

    pdu->iovs[0].iov_len = sizeof(neupdu_header_t);
    pdu->iovs[0].iov_base = &pdu->header;

    
    if(alloc_mem){

        len = size;
        for(i = 1; i < num; i++){
            pdu->iovs[i].iov_len = len > slice ? slice : len;
            pdu->iovs[i].iov_base = xmalloc(pdu->iovs[i].iov_len);
            if(pdu->iovs[i].iov_base == NULL)
                goto err_out;

            len -= slice;
        }
    }
    
    pdu->num = num;

    pdu->start = 1;
    pdu->start_offset = 0;

    pdu->end = 1;
    pdu->end_offset = 0;

    return pdu;

err_out:

   for(i -= 1; i > 0; i--){
        if(pdu->iovs[i].iov_base != NULL){
            xfree(pdu->iovs[i].iov_base);
        }
    }

    return NULL;
}

neupdu_t *neupdu_new(int size){
    neupdu_t *p;

    p = (neupdu_t*)xmalloc(sizeof(*p) + size);
    if(p == NULL){
        return NULL;
    }
    memset(p, 0, sizeof(*p) + size);
 
    p->end = size; // chunk mode : allocated memory size
    
    return p;
}

void neupdu_free(neupdu_t *pdu){
    int i;

    if(pdu->num != 0){
        for(i = 1; i < pdu->num; i++){
            if(pdu->iovs[i].iov_base != NULL){
                xfree(pdu->iovs[i].iov_base);
                pdu->iovs[i].iov_base = NULL;
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

int neupdu_recv(int sock, neupdu_t **pdu){
    neupdu_header_t header;
    neupdu_t    *p;
    int ret;

    ASSERT(pdu != NULL);

    ret = recv(sock, &header, sizeof(header), MSG_PEEK);
    if(ret != sizeof(header)){
        log_warn("data less than header in socket buffer!\n");
        return ret;
    }
    neupdu_header_ntoh(&header);
    
    if(NEUPDU_GET_TYPE(header.cmd) == NEUPDU_TYPE_CHUNK){
        p = neupdu_new(header.len);
        if(p == NULL){
            log_warn("no more memory for pdu buffer!\n");
            return -ENOMEM;
        }

        ret = recv(sock, &p->header, header.len, 0);
        if(ret != header.len){
            log_warn("data in socket is unaviable!\n");
            neupdu_free(p);
            return -1;
        }

        p->end_offset = ret;
    }else{
        p = neupdu_chunk_new(header.len, NEUPDU_SLICE_MAX, 1);
        if(p == NULL){
            log_warn("no more memory for pdu buffer!\n");
            return -ENOMEM;
        }
            
        ret = readv(sock, p->iovs, p->num);
        if(ret != header.len){
            log_warn("data in socket is unaviable!\n");
            neupdu_free(p);
            return -1;
        }
        p->end = p->num;
        p->end_offset = header.len % NEUPDU_SLICE_MAX;
    }
    
    neupdu_header_ntoh(&p->header);
    *pdu = p;

    return 0;
}

int neupdu_send(int sock, neupdu_t *pdu){
    ssize_t ret;
    uint32_t len;
    
    ASSERT(pdu != NULL);

    len = pdu->header.len;

    neupdu_header_hton(&pdu->header);

    if(pdu->num != 0){
        pdu->iovs[pdu->end].iov_len = pdu->end_offset;
        ret = writev(sock, pdu->iovs, pdu->end);
    }else{
        ret = write(sock, &pdu->header, len);
    }

    if(ret != len){
        log_warn("only partial data be sending!\n");
        return -1;
    }

    return 0; 
}

int neupdu_read(neupdu_t *pdu, void *buf, int len){
    int size = 0;
    void *p;

    int cped = 0;
    int i;
    
    ASSERT(pdu != NULL);

    if(pdu->num == 0){
        p = pdu->payload + pdu->start_offset;

        size = pdu->end_offset - pdu->start_offset;
        size = size > len ? len : size;

        memcpy(buf, p, size);
        pdu->start += size;

        return size;
    }else{
        for(i = pdu->start; i <= pdu->end; i++){
            p = pdu->iovs[i].iov_base + pdu->start_offset;
            if(i == pdu->end){
                cped = pdu->end_offset - pdu->start_offset;
            }else{
                cped = pdu->iovs[i].iov_len - pdu->start_offset;
            }
            
            cped = cped > len ? len : cped;
            len -= cped;
            size += cped;
            
            memcpy(buf, p, cped);
            
            pdu->start += 1;
            pdu->start_offset = 0;

            if(len <= 0){
                return size;
            }
        }
    }
 
    return 0;
}

int neupdu_write(neupdu_t *pdu, const void *buf, int len){
    int buf_size;
    void *p;
    int i;
    int cped = 0;

    ASSERT((pdu != NULL) && (buf != NULL));

    if(pdu->num == 0){
        buf_size = pdu->end - pdu->end_offset;
        len = buf_size > len ? len : buf_size;
        p = pdu->payload + pdu->end_offset;

        memcpy(p, buf, len);
        pdu->end_offset += len;
        return len;
    }else{
        for(i = pdu->end; i < pdu->num; i++){
            buf_size = pdu->iovs[i].iov_len - pdu->end_offset;
            buf_size = buf_size > len ? len : buf_size;
            p = pdu->iovs[i].iov_base + pdu->end_offset;
            memcpy(p, buf, buf_size);
            pdu->end_offset += buf_size;
            len -= buf_size;
            cped += buf_size;

            if(len > 0){
                if(pdu->iovs[i].iov_base == NULL){
                    pdu->iovs[i].iov_base = xmalloc(NEUPDU_SLICE_MAX);
                    if(pdu->iovs[i].iov_base == NULL){
                        return cped;
                    }
                    pdu->iovs[i].iov_len = NEUPDU_SLICE_MAX;
                }
                pdu->end += 1;
                pdu->end_offset = 0;
                continue;
            }

            return cped;
        }

        return 0;   
    }

    return 0;
}

