#include "PORT_com.h"
#include "VM_device.h"
#include "dataMemory.h"

static int _writeHandle(device_t *self, args_t *args_in)
{
    int err = 0;

    if (args_in->size(args_in) == 1)
    {
        char *str_send = NULL;
        if (0 != args_in->getStrByIndex(args_in, 0, &str_send))
        {
            err = 1;
            goto exit;
        }
        PORT_send_to_com(2, str_send);
    }

exit:
    return err;
}

static void init_zigbee(device_t *self, args_t *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
    self->_writeHandle = _writeHandle;
}

device_t *New_device_zigbee(args_t *args)
{
    device_t *self = New_device(args);
    self->init = init_zigbee;
    self->init(self, args);
    return self;
}