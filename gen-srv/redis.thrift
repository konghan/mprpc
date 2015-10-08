

struct redis_req{
    1:int req_op
    2:option string key
    3:option string field
    4:option int step
}

struct redis_rsp{
    1:int status
    2:option string value
}

service redisApi{
    1:char *lget(char *key)
    2:int lset(char *key, char *val)
    3:int mget(list<redis_req> reqs)
    4:list<redis_rsp>  mset(list<redis_req> reqs)
} 


