#include "dataLink.h"
#include "dataMemory.h"

static void deinit(Link *self)
{
    DynMemPut(self->mem);
    LinkNode *nowNode = self->firstNode;
    while (NULL != nowNode)
    {
        nowNode->deinit(nowNode);
        nowNode = nowNode->nextNode;
    }
}

static void add(Link *self, void *contant, void (*_contantDinit)(void *contant))
{
    LinkNode *NewNode = New_linkNode(NULL);
    NewNode->contant = contant;
    NewNode->_contantDinit = _contantDinit;
    NewNode->id = self->TopId;
    self->TopId++;

    // old first node become new second node
    LinkNode *secondNode = self->firstNode;

    // change the first node to new node
    self->firstNode = NewNode;

    // link the new first node and second node
    if (NULL != secondNode)
    {
        secondNode->priorNode = self->firstNode;
    }
    self->firstNode->nextNode = secondNode;
}

static int size(Link *self)
{
    LinkNode *NowNode;
    int size = 0;
    NowNode = self->firstNode;
    while (NULL != NowNode)
    {
        size++;
        NowNode = NowNode->nextNode;
    }
    return size;
}

static LinkNode *findNodeById(Link *self, long long id)
{
    LinkNode *nodeNow = self->firstNode;
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

static void init(Link *self, void *args)
{
    /* attrivute */
    self->firstNode = NULL;
    self->TopId = 0;

    /* operation */
    self->deinit = deinit;
    self->add = add;
    self->size = size;
    self->findNodeById = findNodeById;

    /* object */

    /* override */
}

Link *New_link(void *args)
{
    DMEM *mem = DynMemGet(sizeof(Link));
    Link *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
