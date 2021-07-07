#ifndef _link2__H
#define _link2__H
#include "dataLinkNode.h"
#include "dataMemory.h"

typedef struct Class_link Link;
struct Class_link
{
    /* attribute */
    DMEM *mem;
    LinkNode *firstNode;
    long long TopId;

    /* operation */
    void (*deinit)(Link *self);
    void (*init)(Link *self, void *args);
    void (*add)(Link *self, void *contant, void (*_contantDinit)(void *contant));
    LinkNode *(*findNodeById)(Link *self, long long id);

    int (*size)(Link *self);
    /* virtual operation */

    /* object */
};

Link *New_link(void *args);
#endif
