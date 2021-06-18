#include "VM_device.h"
#include "dataArgs.h"
#include "dataMemory.h"

static void deinit(device_t *self)
{
    DynMemPut(self->mem);
}

static int read(device_t *self, args_t *args_in, args_t *args_out)
{
    int err = 0;
    //    if (NULL == args_in)
    //    {
    //        err = 1;
    //        goto exit;
    //    }
    if (NULL == args_out)
    {
        err = 2;
        goto exit;
    }

    if (0 != self->_read_handle(self, args_in, args_out))
    {
        err = 3;
    }

    goto exit;

exit:
    return err;
}

static int _read_handle(device_t *self, args_t *args_in, args_t *args_out)
{
    int err = 0;

    for (int i = 0; i < args_in->size(args_in); i++)
    {
        char *arg = NULL;
        if (0 != args_in->getStrByIndex(args_in, i, &arg))
        {
            err = 1;
            goto exit;
        }
        if (0 != args_out->pushStrWithDefaultName(args_out, arg))
        {
            err = 2;
            goto exit;
        }
    }
    goto exit;

exit:
    return err;
}

static int write(device_t *self, args_t *args_in)
{
    int err = 0;
    if (NULL == args_in)
    {
        err = 1;
        goto exit;
    }
    if (0 == args_in->size(args_in))
    {
        err = 3;
        goto exit;
    }
    if (0 != self->_writeHandle(self, args_in))
    {
        err = 2;
        goto exit;
    }
exit:
    return err;
}

static int _writeHandle(device_t *self, args_t *args_in)
{
    //_writeHandle not be ovrride
    int err = -1;
    goto exit;
exit:
    return err;
}

static float read_float(device_t *self, args_t *args_in)
{
    float val;
    args_t *args_out = New_args(NULL);
    self->read(self, args_in, args_out);
    // the first arg is float
    val = args_out->getFloatByIndex(args_out, 0);
    goto exit;

exit:
    args_out->dinit(args_out);
    return val;
}

static int write_str(device_t *self, char *str)
{
    int err = 0;
    args_t *args_in = New_args(NULL);
    args_in->pushStrWithDefaultName(args_in, str);
    // write str to the first arg
    self->write(self, args_in);
    goto exit;
exit:
    args_in->dinit(args_in);
    return err;
}

static int print(device_t *self, char *str, float val)
{
    char buff[256] = {0};
    int err = 0;
    sprintf(buff, str, val);
    self->write_str(self, buff);
    goto exit;

exit:
    return err;
}

static void init(device_t *self, args_t *args)
{
    /* attrivute */
    self->context = self;

    /* operation */
    self->dinit = deinit;
    self->read = read;
    self->write = write;
    self->read_float = read_float;
    self->write_str = write_str;
    self->print = print;

    /* object */

    /* override */
    self->_read_handle = _read_handle;
    self->_writeHandle = _writeHandle;
}

device_t *New_device(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(device_t));
    device_t *device = mem->addr;
    device->mem = mem;
    device->init = init;
    device->init(device, args);
    return device;
}