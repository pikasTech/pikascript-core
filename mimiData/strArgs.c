#include "strArgs.h"
#include "dataString.h"

Args *New_strBuff(void)
{
    return New_args(NULL);
}

char *strsGetLastToken(Args *buffs, char *argPath, char sign)
{
    int size = strGetSize(argPath);
    char *buff = args_getBuff(buffs, size);
    return strGetLastToken(buff, argPath, sign);
}

char *strsCut(Args *buffs, char *strIn, char startSign, char endSign)
{
    int size = strGetSize(strIn);
    char *buff = args_getBuff(buffs, size);
    return strCut(buff, strIn, startSign, endSign);
}

char *strsDeleteChar(Args *buffs, char *strIn, char ch)
{
    int size = strGetSize(strIn);
    return strDeleteChar(args_getBuff(buffs, size), strIn, ch);
}

char *strsGetFirstToken(Args *buffs, char *strIn, char sign)
{
    int size = strGetSize(strIn);
    return strGetFirstToken(args_getBuff(buffs, size), strIn, sign);
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