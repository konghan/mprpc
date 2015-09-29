
#include "redis-struct-gen.h"

#include <assert.h>

/*
struct redis_req{
    1:int req_op
    2:option string key
    3:option string field
    4:option int step
}
*/

struct redis_req{
    int req_op;
    char *key;
    char *field;
    int step;
};
int redis_req_mashal(struct redis_req *req, neupdu_t *pdu){
    assert((req != NULL) && (pdu != NULL));

    
}

int redis_req_unmashal(neupdu_t *pdu, struct redis_req **req){
}


/*
struct redis_rsp{
    1:int status
    2:option string value
}
*/

struct redis_rsp{
    int status;
    char *value;
};
int redis_rsp_mashal(struct redis_rsp *rsp, neupdu_t *pdu){
}
int redis_rsp_unmashal(neupdu_t *pdu, struct redis_rsp **rsp){
}


