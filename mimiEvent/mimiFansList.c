#include "MimiObj.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"

static void init_FansList(MimiObj *self, Args *args)
{
    /* attrivute */
    self->setObj(self, "fansInfo", New_MimiObj_FansInfo);

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_FansList(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    self->init = init_FansList;
    self->init(self, args);
    return self;
}