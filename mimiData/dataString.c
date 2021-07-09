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

void getLastUnitBySign(char *stringIn, char *stringOut, char sign)
{
	int size = strGetSize(stringIn);
	char strOutBuff[256] = {0};
	for (int i = size - 1; i > -1; i--)
	{
		if (stringIn[i] != sign)
		{
			strOutBuff[size - i - 1] = stringIn[i];
		}
		if (stringIn[i] == sign)
		{
			break;
		}
	}
	int buffSize = strGetSize(strOutBuff);
	for (int i = 0; i < buffSize ; i++)
	{
		stringOut[i] = strOutBuff[buffSize - i -1];
	}
}

void getFirstUnitBySign(char *stringIn, char *stringOut, char sign)
{
	int size = strGetSize(stringIn);
	for (int i = 0; i < size; i++)
	{
		if (stringIn[i] != sign)
		{
			stringOut[i] = stringIn[i];
		}
		if (stringIn[i] == sign)
		{
			break;
		}
	}
}

int devideStringBySign(char *string, char **argv, char sign)
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
