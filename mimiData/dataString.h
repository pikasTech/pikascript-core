#ifndef __MY_TEST_TOOLS_H 
#define __MY_TEST_TOOLS_H 

unsigned short int strGetSize(char *pData);
unsigned short int strGetSize_unlimited(char *pData);
char *strPrint_unlimited(char *strOut, char *pData);
char *strPrint(char *strOut, char *pData);
int isStartWith(char *str, char *strStart);
char *strDeleteEnter(char *str);
char *strPrintWithSize(char *strOut, char *pData, int Size);
int mimiStrEqu(char *str1, char *str2);
void mimiStrRemovePrefix(char *inputStr, char *prefix, char *outputStr);
int devideStringBySign(char *string, char **argv, char sign);
void getFirstUnitBySign(char *stringIn, char *stringOut, char sign);
void getLastTokenBySign(char *stringIn, char *stringOut, char sign);

#endif
