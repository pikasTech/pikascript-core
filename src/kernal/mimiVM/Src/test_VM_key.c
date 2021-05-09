#include "test_VM_key.h"
#include "VM_key.h"
#include <stdio.h>

struct callBack_arg_test
{
    int isShow;
    int callBack_runTimes;
};

static void key_callback(vkey_t *key, void *arg)
{
    struct callBack_arg_test *cb_arg = arg;
    int isShow = cb_arg->isShow;
    if (isShow)
    {
        printf("key_callback runing...\r\n");
    }
    cb_arg->callBack_runTimes++;
}

static int pressGetLow(void)
{
    return 0;
}

static int pressGetHigh(void)
{
    return 1;
}

int TEST_VM_key(int isShow)
{
    struct callBack_arg_test cb_arg = {isShow, 0};
    vkey_t *key = VM_key_init();
    key->callback = key_callback;
    key->callBack_arg = (void *)&cb_arg;
    key->PORT_pressGet = pressGetLow;

    key->update_port(key, 0);
    key->update_callBack(key, 3000);
    key->update_callBack(key, 6000);
    if (0 != cb_arg.callBack_runTimes)
    {
        return 1;
    }
    key->PORT_pressGet = pressGetHigh;
    key->update_port(key, 9000);
    key->update_callBack(key, 12000);
    key->update_callBack(key, 16000);
    key->update_callBack(key, 18000);
    key->update_port(key, 21000);
    key->update_callBack(key, 24000);
    key->update_callBack(key, 27000);
    key->update_callBack(key, 30000);

    if (2 != cb_arg.callBack_runTimes)
    {
        return 2;
    }

    key->deinit(key);
    return 0;
}