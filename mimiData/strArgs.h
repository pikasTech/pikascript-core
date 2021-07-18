#ifndef __STR_ARGS__H
#define __STR_ARGS__H
#include "dataArgs.h"
Args *New_strBuff(void);
char *strsGetLastToken(Args *buffs, char *arg_Path, char sign);
#endif