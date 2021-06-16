#include "test_link2.h"
#include "dataLink.h"
#include "dataTest.h"
#include <stdio.h>

int TEST_link2(int isShow)
{
    int err = 0;
    link_t *link = New_link(NULL);
    dataTest_t *data1 = New_dataTest(NULL);
    dataTest_t *data2 = New_dataTest(NULL);

    data1->a = 2;
    data1->b = 5;
    link->add(link, data1, (void *)data1->dinit);
    data2->a = 4;
    data2->b = 21;
    link->add(link, data2, (void *)data2->dinit);

    dataTest_t *data1InLink = link->findNodeById(link, 0)->contant;
    dataTest_t *data2InLink = link->findNodeById(link, 1)->contant;

    if (isShow)
    {
        printf("the a of data1 in link is:%d\r\n", data1InLink->a);
        printf("the b of data1 in link is:%d\r\n", data1InLink->b);
        printf("the a of data2 in link is:%d\r\n", data2InLink->a);
        printf("the b of data2 in link is:%d\r\n", data2InLink->b);
        printf("the size of link is: %d\r\n", link->size(link));
    }

    if (2 != data1InLink->a)
    {
        err = 1;
        goto exit;
    }

    if (5 != data1InLink->b)
    {
        err = 2;
        goto exit;
    }

    if (4 != data2InLink->a)
    {
        err = 3;
        goto exit;
    }

    if (21 != data2InLink->b)
    {
        err = 4;
        goto exit;
    }

    if (2 != link->size(link))
    {
        err = 5;
        goto exit;
    }

    goto exit;
exit:
    link->dinit(link);
    return err;
}