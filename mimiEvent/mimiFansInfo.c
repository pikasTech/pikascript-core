#include "MimiObj.h"
#include "dataMemory.h"

static void init_FansInfo(MimiObj *self, Args *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_FansInfo(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    self->init = init_FansInfo;
    self->init(self, args);
    return self;
}
