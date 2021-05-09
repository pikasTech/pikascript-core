#include "test_dataQueue.h"
#include "dataQueue.h"
#include <stdio.h>

int TEST_dataQueue(int isShow)
{
    queue_t *queue = dataQueue_init();
    int data1 = 1, data2 = 2, data3 = 5;
    int dataOut1, dataOut2, dataOut3;
    queue->push(queue, (void *)&data1);
    queue->push(queue, (void *)&data2);
    queue->push(queue, (void *)&data3);

    dataOut1 = *((int *)(queue->pop(queue)));
    dataOut2 = *((int *)(queue->pop(queue)));
    dataOut3 = *((int *)(queue->pop(queue)));

    queue->deinit(queue);
    if(isShow)
    {
        printf("The value of out1: %d\r\n",dataOut1);
        printf("The value of out2: %d\r\n",dataOut2);
        printf("The value of out3: %d\r\n",dataOut3);
    }
    if ((dataOut1 != data1) || (dataOut2 != data2) || (dataOut3 != data3))
    {
        return 1;
    }
    return 0;
}