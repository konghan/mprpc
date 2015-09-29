/*
 * Copyright (c) 2014, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */

#include "fixarray.h"
#include "osapi.h"

#include <errno.h>
#include <string.h>

int fixarray_init(fixarray_t *fa, int len){
    fa->items = (void **)xmalloc(sizeof(void*)*len);
    if(fa->items == NULL)
        return -ENOMEM;
    memset(fa->items, 0, sizeof(void*)*len);

    fa->len = len;
    fa->num = 0;

    return 0;
}

int fixarray_fini(fixarray_t *fa){
    if(fa->items != NULL){
        xfree(fa->items);
        fa->items = NULL;
    }

    fa->len = 0;
    fa->num = 0;

    return 0;
}

int fixarray_create(int len, fixarray_t **fa){
    fixarray_t *f;

    f = xmalloc(sizeof(*f));
    if(f == NULL){
        return -ENOMEM;
    }
    
    if(fixarray_init(f, len)!= 0){
        xfree(f);
        return -ENOMEM;
    }

    *fa = f;
    
    return 0;
}

int fixarray_destroy(fixarray_t *fa){
    fixarray_fini(fa);
    xfree(fa);
    
    return 0;
}

int fixarray_get(fixarray_t *fa, int idx, void **obj){
    void *ptr;

    if(idx >= fa->len)
        return -EINVAL;

    ptr = fa->items[idx];
    if(ptr == NULL)
        return -ENOENT;

    *obj = ptr;

    return 0;
}

int fixarray_set(fixarray_t *fa, int idx, void *obj){
    void *ptr;

    if(idx >= fa->len)
        return -EINVAL;

    ptr = fa->items[idx];
    if(ptr == NULL){
        fa->num++;
    }

    fa->items[idx] = obj;

    return 0;
}

int fixarray_append(fixarray_t *fa, void *obj){
    void *ptr;

    if(fa->num >= fa->len)
        return -EINVAL;
    
    ptr = fa->items[fa->num];
    if(ptr != NULL)
        return -EINVAL;

    fa->items[fa->num] = obj;
    fa->num++;

    return 0;
}

int fixarray_clear(fixarray_t *fa, int idx){
    void *ptr;

    if(idx >= fa->len)
        return -EINVAL;

    ptr = fa->items[idx];
    if(ptr == NULL)
        return 0;

    fa->num --;
    fa->items[idx] = NULL;

    return 0;
}

int fixarray_zip(fixarray_t *fa){
    int i, s = 0;

    for(i = 0; i < fa->len; i++){
        if(fa->items[i] != NULL){
            if(s != i){
                fa->items[s] = fa->items[i];
                fa->items[i] = NULL;
                s++;
            }else{
                s++;
            }
        }
    }

    return 0;
}


