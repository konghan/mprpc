/*
 * Copyright (c) 2014, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */

#ifndef __FIXARRAY_H__
#define __FIXARRAY_H__

typedef struct _fixarray{
    int len;
    int num;
    void **items;
}fixarray_t;

int fixarray_init(fixarray_t *fa, int len);
int fixarray_fini(fixarray_t *fa);
int fixarray_create(int len, fixarray_t **fa);
int fixarray_destroy(fixarray_t *fa);

int fixarray_get(fixarray_t *fa, int idx, void **obj);
int fixarray_set(fixarray_t *fa, int idx, void *obj);
int fixarray_append(fixarray_t *fa, void *obj);

int fixarray_clear(fixarray_t *fa, int idx);
int fixarray_zip(fixarray_t *fa);

static inline int fixarray_length(fixarray_t *fa){
    return fa->len;
}

static inline int fixarray_num(fixarray_t *fa){
    return fa->num;
}

#endif // __FIXARRAY_H__


