#include "dataLink.h"
#include "dataMemory.h"

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

static linkNode_t *findNodeById(link_t *self, long long id)
{
    linkNode_t *nodeNow = self->firstNode;
    while (1)
    {
        if (nodeNow->id == id)
        {
            return nodeNow;
        }
        if (nodeNow->nextNode == NULL)
        {
            return NULL;
        }
        nodeNow = nodeNow->nextNode;
    }
}

static void init(link_t *self, void *args)
{
    /* attrivute */
    self->firstNode = NULL;
    self->TopId = 0;

    /* operation */
    self->dinit = deinit;
    self->add = add;
    self->size = size;
    self->findNodeById = findNodeById;

    /* object */

    /* override */
}

link_t *New_link(void *args)
{
    DMEM *mem = DynMemGet(sizeof(link_t));
    link_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
