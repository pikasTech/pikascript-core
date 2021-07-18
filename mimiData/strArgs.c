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

char *strsPopToken(Args *buffs, char *tokens, char sign)
{
    int size = strGetSize(tokens);
    char *buff = args_getBuff(buffs, size);
    return strPopToken(buff, tokens, sign);
}

char *strsCopy(Args *buffs, char *source)
{
    int size = strGetSize(source);
    char *buff = args_getBuff(buffs, size);
    return strCopy(buff, source);
}