#include "MimiObj.h"
#include "dataArgs.h"

void method_returnStr(Args *args, char *val)
{
    args_setStr(args, "return", val);
}

void method_returnInt(Args *args, int val)
{
    args_setInt(args, "return", val);
}

void method_returnFloat(Args *args, float val)
{
    args_setFloat(args, "return", val);
}

void method_returnPtr(Args *args, void *val)
{
    args_setPtr(args, "return", val);
}

void method_sysOut(Args *args, char *str)
{
    args_setStr(args, "sysOut", str);
}