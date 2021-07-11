#ifndef __MY_TEST_TOOLS_H 
#define __MY_TEST_TOOLS_H 

/* size */
unsigned short int strGetSize(char *pData);
unsigned short int strGetSize_unlimited(char *pData);
/* append */
char *strAppend(char *strOut, char *pData);
char *strAppend_unlimited(char *strOut, char *pData);
char *strAppendWithSize(char *strOut, char *pData, int Size);
/* cut */
char *strCut(char *strOut, char *strIn, char startSign, char endSign);
/* assert */
int isStartWith(char *str, char *strStart);
int mimiStrEqu(char *str1, char *str2);
/* delete */
char *strDeleteEnter(char *str);
char *strDeleteChar(char *strOut, char *strIn, char ch);
/* prefix */
char *mimiStrRemovePrefix(char *inputStr, char *prefix, char *outputStr);
/* token */
int getToken(char *string, char **argv, char sign);
char *getFirstToken(char *strOut, char *strIn, char sign);
char *getLastToken(char *strOut, char *strIn, char sign);
char *strClear(char *str);

#endif
