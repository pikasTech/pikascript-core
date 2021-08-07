#include "SysObj.h"
#include "dataMemory.h"

static void init_Mailbox(MimiObj *self, Args *args)
{
    /* attribute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_Mailbox(Args *args)
{
    MimiObj *self = New_MimiObj_sys(args);
    init_Mailbox(self, args);
    return self;
}
