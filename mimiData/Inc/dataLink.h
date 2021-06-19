#ifndef _link2__H
#define _link2__H
#include "dataArgsConst.h"
#include "dataLinkNode.h"
#include "dataMemory.h"

typedef struct Class_link link_t;
struct Class_link
{
    /* attribute */
    DMEM *mem;
    linkNode_t *firstNode;
    long long TopId;

    /* operation */
    void (*dinit)(link_t *self);
    void (*init)(link_t *self, argsConst_t *args);
    void (*add)(link_t *self, void *contant, void (*_contantDinit)(void *contant));
    void *(*tranverse)(
        link_t *self,
        void *(*fun)(linkNode_t *node, argsConst_t *args),
        argsConst_t *args);

    linkNode_t *(*findNodeById)(link_t *self, long long id);

    int (*size)(link_t *self);
    /* virtual operation */

    /* object */
};

link_t *New_link(argsConst_t *args);
#endif
