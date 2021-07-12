#include "dataString.h"
#include <stdio.h>

char *strPrintWithSize_unlimited(char *strOut, char *pData, int Size)
{
	int strOut_i = strGetSize_unlimited(strOut);
	for (int i = 0; i < Size; i++)
	{
		strOut[strOut_i + i] = pData[i];
	}
	strOut_i += Size;
	// add \0 to the end of strOut
	strOut[strOut_i] = 0;

	return strOut;
}

char *strCut(char *strOut, char *strIn, char startSign, char endSign)
{
	int Size = strGetSize(strIn);
	int isStart = 0;
	int isEnd = 0;
	int iOut = 0;
	for (int i = 0; i < Size; i++)
	{
		if (!isStart && (strIn[i] == startSign))
		{
			isStart = 1;
			continue;
		}
		if (isStart && (strIn[i] == endSign))
		{
			isEnd = 1;
			break;
		}
		if (isStart)
		{
			strOut[iOut] = strIn[i];
			iOut++;
		}
	}
	/* add \0 */
	strOut[iOut] = 0;
	if (isStart && isEnd)
	{
		/* succeed */
		return strOut;
	}
	/* faild */
	return NULL;
}

char *strDeleteEnter(char *str)
{
	for (int i = 0; i < 256; i++)
	{
		if (0 == str[i])
		{
			return str;
		}
		if ('\n' == str[i])
		{
			str[i] = 0;
			return str;
		}
	}
	return str;
}

char *strDeleteChar(char *strOut, char *strIn, char ch)
{
	int iOut = 0;
	for (int i = 0; i < strGetSize(strIn); i++)
	{
		if (ch == strIn[i])
		{
			continue;
		}
		strOut[iOut] = strIn[i];
		iOut++;
	}
	/* add \0 */
	strOut[iOut] = 0;
	return strOut;
}

char *strAppendWithSize(char *strOut, char *pData, int Size)
{
	unsigned short int strOut_i = strGetSize(strOut);
	for (int i = 0; i < Size; i++)
	{
		strOut[strOut_i + i] = pData[i];
	}
	strOut_i += Size;
	// add \0 to the end of strOut
	strOut[strOut_i] = 0;

	return strOut;
}

unsigned short int strGetSize_unlimited(char *pData)
{
	unsigned short int Size = 0;
	for (int i = 0; pData[i] != 0; i++)
	{
		Size++;
	}
	return Size;
}

unsigned short int strGetSize(char *pData)
{
	unsigned short int Size = 0;
	if (NULL == pData)
	{
		return 0;
	}
	for (int i = 0; (i < 256) && (pData[i] != 0); i++)
	{
		Size++;
	}
	return Size;
}

char *strAppend_unlimited(char *strOut, char *pData)
{
	unsigned short int Size = 0;
	Size = strGetSize_unlimited(pData);
	return strPrintWithSize_unlimited(strOut, pData, Size);
}

char *getLastToken(char *strOut, char *strIn, char sign)
{
	int size = strGetSize(strIn);
	char strOutBuff[256] = {0};
	for (int i = size - 1; i > -1; i--)
	{
		if (strIn[i] != sign)
		{
			strOutBuff[size - i - 1] = strIn[i];
		}
		if (strIn[i] == sign)
		{
			break;
		}
	}
	int buffSize = strGetSize(strOutBuff);
	for (int i = 0; i < buffSize; i++)
	{
		strOut[i] = strOutBuff[buffSize - i - 1];
	}
	return strOut;
}

char *popToken(char *strOut, char *strIn, char sign)
{
	int getSign = 0;
	int iPoped = 0;
	int iOut = 0;
	int size = strGetSize(strIn);
	int i;
	for (i = 0; i < size; i++)
	{
		if (getSign)
		{
			strIn[iPoped++] = strIn[i];
			continue;
		}
		if (strIn[i] != sign)
		{
			strOut[iOut++] = strIn[i];
			continue;
		}
		if (strIn[i] == sign)
		{
			getSign = 1;
			continue;
		}
	}
	strOut[iOut] = 0;
	strIn[iPoped] = 0;
	return strOut;
}

char *getFirstToken(char *strOut, char *strIn, char sign)
{
	int size = strGetSize(strIn);
	for (int i = 0; i < size; i++)
	{
		if (strIn[i] != sign)
		{
			strOut[i] = strIn[i];
		}
		if (strIn[i] == sign)
		{
			break;
		}
	}
	return strOut;
}

int getToken(char *string, char **argv, char sign)
{
	int argc = 0;
	int i = 0;
	//arg_i point to the arg operated now
	int arg_i = 0;
	// if not found ' ', then put chars from CMD to argv_tem
	int char_i = 0;
	for (i = 0; (i < strGetSize(string)); i++)
	{
		if (string[i] != sign)
		{
			argv[arg_i][char_i] = string[i];
			char_i++;
		}
		if (string[i] == sign)
		{
			// write '\0' to the end of argv
			argv[arg_i][char_i] = 0;
			arg_i++;
			char_i = 0;
		}
		// write '\0' to the end of last argv
		argv[arg_i][char_i] = 0;
	}
	argc = arg_i + 1;
	return argc;
}

char *strAppend(char *strOut, char *pData)
{
	unsigned short int Size = 0;
	Size = strGetSize(pData);
	return strAppendWithSize(strOut, pData, Size);
}

int isStartWith(char *str, char *strStart)
{
	unsigned short int size = strGetSize(strStart);
	unsigned short int CMDName_get = 1;
	for (int i = 0; i < size; i++)
	{
		if (str[i] != strStart[i])
		{
			CMDName_get = 0;
		}
	}
	return CMDName_get;
}

int mimiStrEqu(char *str1, char *str2)
{
	int size1 = strGetSize(str1);
	int size2 = strGetSize(str2);
	if (size1 != size2)
	{
		return 0; // not equal
	}
	for (int i = 0; i < size1; i++)
	{
		if (str1[i] != str2[i])
		{
			return 0; // not equal
		}
	}
	return 1;
}

char *mimiStrRemovePrefix(char *inputStr, char *prefix, char *outputStr)
{
	if (!isStartWith(inputStr, prefix))
	{
		return NULL;
	}

	for (int i = strGetSize(prefix); i < strGetSize(inputStr); i++)
	{
		outputStr[i - strGetSize(prefix)] = inputStr[i];
	}
	return outputStr;
}

char *strClear(char *str)
{
	for (int i = 0; i < sizeof(str); i++)
	{
		str[i] = 0;
	}
	return str;
}