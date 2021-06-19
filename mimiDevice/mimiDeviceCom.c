#include "mimiProcess.h"
#include "dataMemory.h"

static void init_deviceCom(mimiProcess_t *self, args_t *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
}

mimiProcess_t *New_mimiProcess_deviceCom(args_t *args)
{
    mimiProcess_t *self = New_mimiProcess(args);
    self->init = init_deviceCom;
    self->init(self, args);
    return self;
}