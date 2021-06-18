#include "PORT_com.h"
#include "VM_device.h"
#include "dataMemory.h"
#include "dataString.h"

static int _writeHandle(device_t *self, args_t *args_in)
{
    char *str = NULL;
    args_in->getStrByIndex(args_in, 0, &str);
    PORT_send_to_com(3, str);
    return 0;
}

static void init_blueTeeth(device_t *self, args_t *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
    self->_writeHandle = _writeHandle;
}

device_t *New_device_blueTeeth(args_t *args)
{
    device_t *self = New_device(args);
    self->init = init_blueTeeth;
    self->init(self, args);
    return self;
}