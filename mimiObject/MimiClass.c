#include "MimiObj.h"
#include "dataMemory.h"
#include "strArgs.h"
#include "dataString.h"

static void init_class(MimiObj *self, Args *args)
{
    /* attribute */

    /* operation */

    /* object */

    /* override */
}

MimiObj *New_MimiObj_class(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_class(self, args);
    return self;
}