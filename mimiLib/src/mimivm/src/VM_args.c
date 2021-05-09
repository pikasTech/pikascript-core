#include "VM_args.h"
#include "VM_memory.h"
#include "mimiStr.h"
#include "string.h"
#include <stdlib.h>

static void deinit(args_t *self)
{
    DynMemPut(self->mem);

    for (int i = 0; i < 32; i++)
    {
        if (NULL != self->list_mem[i])
        {
            DynMemPut(self->list_mem[i]);
        }
    }
}

static int get(args_t *self, int index, char **arg_get)
{
    int errCode = 0;
    if (index > 32)
    {
        errCode = 2;
        goto exit;
    }
    if (index + 1 > self->count)
    {
        errCode = 1;
        goto exit;
    }
    *arg_get = self->list[index];
    goto exit;

exit:
    return errCode;
}

static int put(args_t *self, char *arg_put)
{
    int errCode = 0;
    if (NULL == arg_put)
    {
        errCode = 1;
        goto exit;
    }
    self->list_mem[self->count] = DynMemGet(sizeof(char) * 32);
    self->list[self->count] = self->list_mem[self->count]->addr;
    char *argNew = self->list[self->count];
    memcpy(argNew, arg_put, 32);
    self->count++;
    goto exit;

exit:
    return errCode;
}

static int put_float(args_t *self, float arg_int)
{
    char arg_put[32] = {0};
    sprintf(arg_put, "%f", arg_int);
    self->put(self, arg_put);
    return 0;
}

float get_float(args_t *self, int index)
{
    float val = 0;
    char *arg_get = NULL;
    self->get(self, index, &arg_get);
    val = atof(arg_get);
    goto exit;
exit:
    return val;
}

static void init(args_t *self, int argc, char **argv)
{
    /* attrivute */
    self->context = self;
    self->count = 0;
    for (int i = 0; i < 32; i++)
    {
        self->list[i] = NULL;
        self->list_mem[i] = NULL;
    }

    /* operation */
    self->dinit = deinit;
    self->put = put;
    self->get = get;
    self->get_float = get_float;
    self->put_float = put_float;

    /* object */

    /* override */
}

args_t *New_args(int argc, char **argv)
{
    DMEM *mem = DynMemGet(sizeof(args_t));
    args_t *args = mem->addr;
    args->mem = mem;
    args->init = init;
    args->init(args, argc, argv);
    return args;
}