#include "MimiObj.h"
#include "dataMemory.h"

static void init_Mailbox(MimiObj *self, Args *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_Mailbox(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    self->init = init_Mailbox;
    self->init(self, args);
    return self;
}
