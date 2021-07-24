#include "sysObj.h"
#include "dataMemory.h"

static void init_FansInfo(MimiObj *self, Args *args)
{
    /* attribute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_FansInfo(Args *args)
{
    MimiObj *self = New_MimiObj_sys(args);
    init_FansInfo(self, args);
    return self;
}
