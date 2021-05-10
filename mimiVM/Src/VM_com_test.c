#include "VM_com_test.h"
#include "VM_com.h"
#include "dataString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int test_send(int isShow)
{
    // create the com1 and com2 objects
    VM_com_t *com1 = VM_com_init_v2();
    VM_com_t *com2 = VM_com_init_v2();

    // set the name of com2
    strPrint(com2->name, "com2");
    // set the driver send of com2
    com2->DriverSend = VMdriver_comTransmitHuart2;

    com1->send(com1, "test send \r\n");
    com2->send(com2, "test send com2\r\n");
    if (isShow)
    {
        printf("the TX_buf of com1 = %s\r\n", com1->TX_buff);
        printf("out length = %d\r\n", strGetSize(com1->TX_buff));
        printf("the TX_buf of com2 = %s\r\n", com2->TX_buff);
        printf("out length = %d\r\n", strGetSize(com2->TX_buff));
    }
    if (12 != strGetSize(com1->TX_buff))
    {
        return 1;
    }
    if (16 != strGetSize(com2->TX_buff))
    {
        return 2;
    }
    com1->DeInit(com1);
    com2->DeInit(com2);
    return 0;
}

int TEST_VM_com_test_list(int isShow)
{
    if (0 != test_send(isShow))
    {
        return 1;
    }
    return 0;
}