#include "dataLink.h"
#include "dataMemory.h"
#include "dataArgsConst.h"

static void deinit(link_t *self)
{
    DynMemPut(self->mem);
    linkNode_t *nowNode = self->firstNode;
    while (NULL != nowNode)
    {
        nowNode->dinit(nowNode);
        nowNode = nowNode->nextNode;
    }
}

static void add(link_t *self, void *contant, void (*_contantDinit)(void *contant))
{
    linkNode_t *NewNode = New_linkNode(NULL);
    NewNode->contant = contant;
    NewNode->_contantDinit = _contantDinit;
    NewNode->id = self->TopId;
    self->TopId++;

    // old first node become new second node
    linkNode_t *secondNode = self->firstNode;

    // change the first node to new node
    self->firstNode = NewNode;

    // link the new first node and second node
    if (NULL != secondNode)
    {
        secondNode->priorNode = self->firstNode;
    }
    self->firstNode->nextNode = secondNode;
}

static int size(link_t *self)
{
    linkNode_t *NowNode;
    int size = 0;
    NowNode = self->firstNode;
    while (NULL != NowNode)
    {
        size++;
        NowNode = NowNode->nextNode;
    }
    return size;
}

static void *getNodeWhenIdMate(linkNode_t *node, argsConst_t *args)
{
    long long id = args->getInt64ByName(args, "id");
    if (node->isId(node, id))
    {
        return node;
    }
    return NULL;
}

static linkNode_t *findNodeById(link_t *self, long long id)
{
    linkNode_t *nodeOut = NULL;
    argsConst_t *args = New_argsConst(NULL);
    args->pushInt64WithName(args, "id", id);
    nodeOut = self->tranverse(self, getNodeWhenIdMate, args);
    args->dinit(args);
    return nodeOut;
}

static void *tranverse(link_t *self,
                       void *(*nodeOperation)(linkNode_t *node, argsConst_t *args),
                       argsConst_t *args)
{
    linkNode_t *node = self->firstNode;
    void *operationOut;
    while (NULL != node)
    {
        operationOut = nodeOperation(node, args);
        if (NULL != operationOut)
        {
            return operationOut;
        }
        node = node->nextNode;
    }
    return NULL;
}

static void init(link_t *self, argsConst_t *args)
{
    /* attrivute */
    self->firstNode = NULL;
    self->TopId = 0;

    /* operation */
    self->dinit = deinit;
    self->add = add;
    self->size = size;
    self->findNodeById = findNodeById;
    self->tranverse = tranverse;

    /* object */

    /* override */
}

link_t *New_link(argsConst_t *args)
{
    DMEM *mem = DynMemGet(sizeof(link_t));
    link_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
