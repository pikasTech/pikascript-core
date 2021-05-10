#include "dataLinkWithNode.h"
#include "dataMemory.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataList.h"

static void deinit_data_default(void *self);
void deinit_node(linkWithNode_t *self);

int linkNode_size(linkWithNode_t *self)
{
    linkWithNode_t *NodeNow;
    int size = 0;
    NodeNow = self;
    while (NULL != NodeNow->next)
    {
        size++;
        // point to the next Node
        NodeNow = NodeNow->next;
    }
    return size;
}

void linkNode_add(linkWithNode_t *self, void *dataNew)
{
    linkWithNode_t *NodeNew;
    linkWithNode_t *NodeFirst;
    DMEM *NodeNewMem;

    NodeNewMem = DynMemGet(sizeof(linkWithNode_t));
    NodeNew = (linkWithNode_t *)(NodeNewMem->addr);
    NodeNew->mem = NodeNewMem;

    NodeFirst = self->next;

    // add the new data to the new node
    NodeNew->data = dataNew;
    // operate the pointer add the new node between the head and first node
    // head do not have prior, so only need to operate the next pointer
    self->next = NodeNew;

    // operate he pointer of new node
    NodeNew->prior = self;

    NodeNew->next = NodeFirst;
}



void *linkNode_traverse(linkWithNode_t *self, void *(*operate)(void *))
{
    linkWithNode_t *NodeNow;
    data_t *dataNow;
    NodeNow = self->next;
    void *out = NULL;

    while (NULL != NodeNow)
    {
        dataNow = (data_t *)NodeNow->data;
        {
            out = operate(dataNow);
            // return when get output
            if (NULL != out)
            {
                return out;
            }
        }
        NodeNow = NodeNow->next;
    }
    return NULL;
}



static void deinit(linkWithNode_t *Head)
{
    // destroy the link (deinit the node one by one)
    Head->destroy(Head);
    // deinit the Head
    DynMemPut(Head->mem);
}

void linkNode_destroy(linkWithNode_t *self)
{
    linkWithNode_t *NodeNow, *NodeNext;
    NodeNow = self->next;

    // if the link is empty, return directly
    if (NULL == NodeNow)
    {
        return;
    }

    while (NULL != NodeNow->next)
    {
        // get hte node next
        NodeNext = NodeNow->next;
        // deinit the data of nodeNow, use the function of Head
        self->port_deinit_data(NodeNow->data);
        // deinit the nodNow, use the function of Head
        self->deinit_node(NodeNow);
        // point to the next Node
        NodeNow = NodeNext;
    }
    // deinit the data of Last Node
    self->port_deinit_data(NodeNow->data);
    // deinit the Last Node
    self->deinit_node(NodeNow);
}

void deinit_node(linkWithNode_t *self)
{
    DynMemPut(self->mem);
}

// if use other data sturct, the port_deinit_data function need to be rewrite
static void deinit_data_default(void *data_noType)
{
    data_t *data = data_noType;
    DynMemPut(data->mem);
    data->mem = NULL;
}



void init(linkWithNode_t *self, list_t *args)
{
    // load the functions
    self->add = linkNode_add;
    self->destroy = linkNode_destroy;
    self->size = linkNode_size;
    self->traverse = linkNode_traverse;
    self->deinit = deinit;
    // should be redefine in user code, set how to deinit the data, becasue the data used in user code is not known by the linkWithNode, it is define by user.
    self->port_deinit_data = deinit_data_default;
    self->deinit_node = deinit_node;

    // init the data
    // the circulate double link
    self->next = NULL;
    self->prior = NULL;
}


linkWithNode_t *New_linkWithNode(list_t *args)
{
    //note: only the Head of link have the operations, other node donot have operations, so wehen operate other node, use the operations of Head.

    // make a new node, and point the List to the node
    DMEM *mem = DynMemGet(sizeof(linkWithNode_t));
    linkWithNode_t *self = (linkWithNode_t *)(mem->addr);
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
