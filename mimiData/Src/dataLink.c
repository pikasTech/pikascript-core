#include "dataLink.h"
#include "dataLinkNode.h"
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

static void addNode(Link *self, void *contant, void (*_contantDinit)(void *contant))
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

static void removeNode(Link *self, void *contant)
{
    LinkNode *nodeToDelete = NULL;
    LinkNode *nodeNow = self->firstNode;
    while (1)
    {
        if (nodeNow->contant == contant)
        {
            nodeToDelete = nodeNow;
            break;
        }
        if (nodeNow->nextNode == NULL)
        {
            // error, node no found
            return;
        }
        nodeNow = nodeNow->nextNode;
    }

    LinkNode *nextNode = nodeToDelete->nextNode;
    LinkNode *priorNode = nodeToDelete->priorNode;

    if (NULL != priorNode)
    {
        priorNode->nextNode = nextNode;
    }

    if (NULL != nextNode)
    {
        nextNode->priorNode = priorNode;
    }

    // deinit the node
    nodeToDelete->deinit(nodeToDelete);
    return;
}

static int getSize(Link *self)
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

static LinkNode *getNode(Link *self, long long id)
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
    self->getSize = getSize;

    self->addNode = addNode;
    self->getNode = getNode;
    self->removeNode = removeNode;

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
