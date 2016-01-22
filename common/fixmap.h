/*
 * Copyright (c) 2014, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */

#ifndef __FIXMAP_H__
#define __FIXMAP_H__

#include "rbtree.h"
#include "osapi.h"

#include <stdint.h>
#include <stddef.h>

typedef struct _fixmap_node{
    struct  rb_node node;
}fixmap_node_t;

typedef int (*fixmap_compare)(void *kfirst, fixmap_node_t *node);

typedef struct _fixmap{
    struct rb_root root;
    int num;

    fixmap_compare  compare;
}fixmap_t;

int fixmap_init(fixmap_t *fm, fixmap_compare compare);
int fixmap_fini(fixmap_t *fm);

int fixmap_get(fixmap_t *fm, void *key, fixmap_node_t **node);

int fixmap_insert(fixmap_t *fm, void *key, fixmap_node_t *node);
int fixmap_remove(fixmap_t *fm, void *key, fixmap_node_t **node);

/*
 * fmap - fixmap_t *
 * fnode - fixmap_node_t *
 */
#define fixmap_for_each(fmap, rbnode, fnode)  \
    for(rbnode = rb_first(&(fmap)->root), (fnode) = rb_entry(rbnode, fixmap_node_t, node);\
            rbnode != NULL;     \
            rbnode = rb_next(rbnode), (fnode) = rb_entry(rbnode, fixmap_node_t, node))


static inline void fixmap_node_init(fixmap_node_t *node){
    rb_init_node(&node->node);
}

static inline int fixmap_num(fixmap_t *fm){
    return fm->num;
}

#endif // __FIXMAP_H__

