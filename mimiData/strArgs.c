#include "strArgs.h"
#include "dataString.h"

Args *New_strBuff(void)
{
    return New_args(NULL);
}

char *strsGetLastToken(Args *buffs, char *arg_Path, char sign)
{
    int size = strGetSize(arg_Path);
    char *buff = args_getBuff(buffs, size);
    return strGetLastToken(buff, arg_Path, sign);
}