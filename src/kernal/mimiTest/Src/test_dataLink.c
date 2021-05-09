#include "test_dataLink.h"
#include "dataLinkWithNode.h"

void *find_a_equal_3(void *data)
{
    data_t *data_typed = (data_t *)data;
    if (data_typed->a == 3)
    {
        return data;
    }
    // NULL means do not return nothing
    return NULL;
}

int linkNode_test(linkWithNode_t *Head, int isShow)
{
    DMEM *mem1, *mem2;
    mem1 = DynMemGet(sizeof(data_t));
    mem2 = DynMemGet(sizeof(data_t));
    data_t *dataNew1 = (data_t *)(mem1->addr);
    data_t *dataNew2 = (data_t *)(mem2->addr);
    dataNew1->mem = mem1;
    dataNew2->mem = mem2;
    int size;

    dataNew1->a = 1;
    dataNew1->b = 2;

    dataNew2->a = 3;
    dataNew2->b = 4;

    // add the data to list
    Head->add(Head, dataNew1);
    Head->add(Head, dataNew2);

    size = Head->size(Head);

    if (isShow)
    {
        printf("size = %d\r\n", size);
    }

    if (2 != size)
    {
        return 1;
    }

    data_t *finded;
    finded = (data_t *)Head->traverse(Head, find_a_equal_3);
    if (isShow)
    {
        printf("finded = %d \r\n", finded->a);
    }
    if (3 != finded->a)
    {
        return 2;
    }
    return 0;
}

int TEST_dataLink(int isShow)
{
    linkWithNode_t *Head = New_linkWithNode(NULL);
    int testOut = 0;
    testOut = linkNode_test(Head, isShow);
    Head->deinit(Head);
    return testOut;
}
