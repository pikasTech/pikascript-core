#include "test_dataLog.h"
#include "dataLog.h"
#include "stdio.h"

int TEST_datalog(int isShow)
{
    Loger *loger = dataLog_init();
    loger->print(loger, "test log \r\n");
    loger->print(loger, "test 2\r\n");
    if (isShow)
    {
        printf("log of data loger is :\r\n%s", loger->log);
        printf("size of data loger is : %d\r\n", loger->log_size);
    }
    if (19 != loger->log_size)
    {
        return 1;
    }
    loger->deinit(loger);
    return 0;
}