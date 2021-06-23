#include "dataString.h"

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

char *strPrintWithSize(char *strOut, char *pData, int Size)
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
	for (int i = 0; (i < 256) && (pData[i] != 0); i++)
	{
		Size++;
	}
	return Size;
}

char *strPrint_unlimited(char *strOut, char *pData)
{
	unsigned short int Size = 0;
	Size = strGetSize_unlimited(pData);
	return strPrintWithSize_unlimited(strOut, pData, Size);
}

char *strPrint(char *strOut, char *pData)
{
	unsigned short int Size = 0;
	Size = strGetSize(pData);
	return strPrintWithSize(strOut, pData, Size);
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

void mimiStrRemovePrefix(char *inputStr, char *prefix, char *outputStr)
{
	if (!isStartWith(inputStr, prefix))
	{
		return;
	}

	for (int i = strGetSize(prefix); i < strGetSize(inputStr); i++)
	{
		outputStr[i - strGetSize(prefix)] = inputStr[i];
	}
}