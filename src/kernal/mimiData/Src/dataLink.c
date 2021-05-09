#include "dataLink.h"
#include "VM_memory.h"
#include <stdio.h>
#include <stdlib.h>

static void deinit_data_default(void *data);
void deinit_node(dataLink_t *Node);

int linkNode_size(dataLink_t *Head)
{
    dataLink_t *NodeNow;
    int size = 0;
    NodeNow = Head;
    while (NULL != NodeNow->next)
    {
        size++;
        // point to the next Node
        NodeNow = NodeNow->next;
    }
    return size;
}

void linkNode_add(dataLink_t *Head, void *dataNew)
{
    dataLink_t *NodeNew;
    dataLink_t *NodeFirst;
    DMEM *NodeNewMem;

    NodeNewMem = DynMemGet(sizeof(dataLink_t));
    NodeNew = (dataLink_t *)(NodeNewMem->addr);
    NodeNew->mem = NodeNewMem;

    NodeFirst = Head->next;

    // add the new data to the new node
    NodeNew->data = dataNew;
    // operate the pointer add the new node between the head and first node
    // head do not have prior, so only need to operate the next pointer
    Head->next = NodeNew;

    // operate he pointer of new node
    NodeNew->prior = Head;

    NodeNew->next = NodeFirst;
}



void *linkNode_traverse(dataLink_t *Head, void *(*operate)(void *))
{
    dataLink_t *NodeNow;
    data_t *dataNow;
    NodeNow = Head->next;
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



static void deinit(dataLink_t *Head)
{
    // destroy the link (deinit the node one by one)
    Head->destroy(Head);
    // deinit the Head
    DynMemPut(Head->mem);
}

void linkNode_destroy(dataLink_t *Head)
{
    dataLink_t *NodeNow, *NodeNext;
    NodeNow = Head->next;

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
        Head->port_deinit_data(NodeNow->data);
        // deinit the nodNow, use the function of Head
        Head->deinit_node(NodeNow);
        // point to the next Node
        NodeNow = NodeNext;
    }
    // deinit the data of Last Node
    Head->port_deinit_data(NodeNow->data);
    // deinit the Last Node
    Head->deinit_node(NodeNow);
}

void deinit_node(dataLink_t *Node)
{
    DynMemPut(Node->mem);
}

// if use other data sturct, the port_deinit_data function need to be rewrite
static void deinit_data_default(void *data_noType)
{
    data_t *data = data_noType;
    DynMemPut(data->mem);
    data->mem = NULL;
}

dataLink_t *dataLink_init(void)
{
    //note: only the Head of link have the operations, other node donot have operations, so wehen operate other node, use the operations of Head.

    // make a new node, and point the List to the node
    DMEM *mem = DynMemGet(sizeof(dataLink_t));
    dataLink_t *Head = (dataLink_t *)(mem->addr);
    Head->mem = mem;

    // load the functions
    Head->add = linkNode_add;
    Head->destroy = linkNode_destroy;
    Head->size = linkNode_size;
    Head->traverse = linkNode_traverse;
    Head->deinit = deinit;
    // should be redefine in user code, set how to deinit the data, becasue the data used in user code is not known by the dataLink, it is define by user.
    Head->port_deinit_data = deinit_data_default;
    Head->deinit_node = deinit_node;

    // init the data
    // the circulate double link
    Head->next = NULL;
    Head->prior = NULL;
    return Head;
}
