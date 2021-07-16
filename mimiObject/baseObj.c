#include "MimiObj.h"
#include "baseObj.h"
#include "dataMemory.h"
#include "dataString.h"

static void init_baseObj(MimiObj *self, Args *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_baseObj(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_baseObj(self, args);
    return self;
}
