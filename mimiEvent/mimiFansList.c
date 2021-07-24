#include "sysObj.h"
#include "baseObj.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"

static void init_FansList(MimiObj *self, Args *args)
{
    /* attribute */
    obj_importAndSetObj(self, "fansInfo", New_MimiObj_FansInfo);

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_FansList(Args *args)
{
    MimiObj *self = New_MimiObj_sys(args);
    init_FansList(self, args);
    return self;
}
