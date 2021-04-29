#include "mimiStr.h"

char *strPrintWithSize_unlimited(char *strOut, char *pData, unsigned short int Size, unsigned short int isClearBuff)
{
	unsigned short int strOut_i = strGetSize_unlimited(strOut);
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

char *strPrintWithSize(char *strOut, char *pData, unsigned short int Size, unsigned short int isClearBuff)
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
	return strPrintWithSize_unlimited(strOut, pData, Size, 0);
}

char *strPrint(char *strOut, char *pData)
{
	unsigned short int Size = 0;
	Size = strGetSize(pData);
	return strPrintWithSize(strOut, pData, Size, 0);
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
