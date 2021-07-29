#ifndef __STR_ARGS__H
#define __STR_ARGS__H
#include "dataArgs.h"
Args *New_strBuff(void);
char *strsGetFirstToken(Args *buffs, char *strIn, char sign);
char *strsGetLastToken(Args *buffs, char *arg_Path, char sign);
char *strsPopToken(Args *buffs, char *tokens, char sign);
char *strsCopy(Args *buffs, char *source);
char *strsDeleteChar(Args *buff, char *strIn, char ch);
char *strsCut(Args *buffs, char *strIn, char startSign, char endSign);
#endif