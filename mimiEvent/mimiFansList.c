#include "MimiObj.h"
#include "baseObj.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"

static void init_FansList(MimiObj *self, Args *args)
{
    /* attrivute */
    obj_newObj(self, "fansInfo", New_MimiObj_FansInfo);

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_FansList(Args *args)
{
    MimiObj *self = New_baseObj(args);
    init_FansList(self, args);
    return self;
}
