#include "fixmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define NUM_NODES 32

struct key {
    fixmap_node_t knode;
    
    char key[32];
};

static int compare_key(void *k, fixmap_node_t *node){
    struct key *kf = (struct key*)k;
    struct key *ks = container_of(node, struct key, knode);

    return strcmp(kf->key, ks->key);
}

int main()
{
    fixmap_t fm;
    fixmap_node_t *fmn;
    struct rb_node *rbnode;
    struct key  *mn[NUM_NODES];
    struct key *tmp;
    int i, rc;

    fixmap_init(&fm, compare_key);

    for (i= 0; i < NUM_NODES; i++) {
        mn[i] = (struct key *)malloc(sizeof(struct key));
        
        snprintf((mn[i])->key, 32, "key%d\n", (int)random());
        
        rc = fixmap_insert(&fm, mn[i], &(mn[i])->knode);

        printf("   %s\n", rc ? "fail" : "ok");
    }
    
    /* list  */
    fixmap_for_each((&fm), rbnode, fmn){
        tmp = container_of(fmn, struct key, knode);
        printf("key: %s\n", tmp->key);
    }

    /* search */
    rc = fixmap_get(&fm, mn[5], &fmn);
    if(rc != 0){
        printf("error : key not found!\n");
        return -1;
    }
    printf("find key 5 : %s\n", (container_of(fmn, struct key, knode))->key);

    /* delete */
    if(fixmap_remove(&fm, mn[6], &fmn) != 0){
        printf("remove node fail\n");
        return -1;
    }
    printf("remove key : %s\n", (container_of(fmn, struct key, knode))->key);
    rc = fixmap_get(&fm, mn[6], &fmn);
    if(rc != 0){
        printf("remove success\n");
    }else{
        printf("remove fail\n");
        return -1;
    }

    printf ("fixmap basic interface ok\n");

    return 0;
}
