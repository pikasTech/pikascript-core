#include "MimiObj.h"
#include "baseObj.h"
#include "dataMemory.h"

static void print(MimiObj *obj, Args *args)
{
    char *res = args_print(args, "argName");
    printf("%s\r\n", res);
}

static void init_baseObj(MimiObj *self, Args *args)
{
    /* attrivute */

    /* operation */
    obj_setMethod(self, "print(argName:string)", print);

    /* object */

    /* override */
}

MimiObj *New_baseObj(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_baseObj(self, args);
    return self;
}