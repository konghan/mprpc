/*
 * Copyright (c) 2014, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */

#include "fixarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct obj_{
    char *name;
    int idx;
}obj_t;

int main(){
    obj_t *obj;
    fixarray_t *fa;
    int i;

    fixarray_create(16, &fa);

    for(i = 0; i < 16; i++){
        obj = malloc(sizeof(*obj));
        obj->name = strdup("obj-name");
        obj->idx = i;

        fixarray_set(fa, i, obj);
    }

    printf("fixarray- len:%d num:%d\n", fa->len, fa->num);

    fixarray_clear(fa, 2);
    fixarray_clear(fa, 8);

    fixarray_zip(fa);

    for(i = 0; i < fa->num; i++){
        if(fixarray_get(fa, i, (void **)&obj) == 0){
            printf("obj index:%d\n", obj->idx);
        }
    }

    printf("fixarray- len:%d num:%d\n", fa->len, fa->num);

    return 0;
}

