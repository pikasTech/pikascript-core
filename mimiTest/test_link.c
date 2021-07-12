#include "test_link.h"
#include "dataLink.h"
#include "dataTest.h"
#include <stdio.h>

int TEST_link2(int isShow)
{
    int err = 0;
    {
        Link *link = New_link(NULL);
        dataTest_t *data1 = New_dataTest(NULL);
        dataTest_t *data2 = New_dataTest(NULL);

        data1->a = 2;
        data1->b = 5;
        link_addNode(link, data1, (void *)data1->deinit);
        data2->a = 4;
        data2->b = 21;
        link_addNode(link, data2, (void *)data2->deinit);

        dataTest_t *data1InLink = link_getNode(link, 0)->contant;
        dataTest_t *data2InLink = link_getNode(link, 1)->contant;

        if (isShow)
        {
            printf("the a of data1 in link is:%d\r\n", data1InLink->a);
            printf("the b of data1 in link is:%d\r\n", data1InLink->b);
            printf("the a of data2 in link is:%d\r\n", data2InLink->a);
            printf("the b of data2 in link is:%d\r\n", data2InLink->b);
            printf("the size of link is: %d\r\n", link_getSize(link));
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

        if (2 != link_getSize(link))
        {
            err = 5;
            goto exit;
        }

        if (isShow)
        {
            printf("the size before remove node:%d\r\n", link_getSize(link));
        }
        link_removeNode(link, data1InLink);
        if (isShow)
        {
            printf("the size after remove node:%d\r\n", link_getSize(link));
        }

        link_deinit(link);
    }

exit:
    return err;
}