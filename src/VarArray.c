#include <stdlib.h>
#include <string.h>
#include "VarArray.h"
void StringArray_Init(StringArray* SArray)
{
	SArray->Array_Num = 0;
	SArray->pArray_Length = 2;
	SArray->pArray = (VarString_Typedef)malloc(SArray->pArray_Length * sizeof(String_Typedef*) );
}
void StringArray_AddString(StringArray* SArray, const String_Typedef* Str)
{
	if(Str == 0)
	{
		return ;
	}
	if (SArray->Array_Num >= SArray->pArray_Length)
	{
		VarString_Typedef Temp = 0;
		SArray->pArray_Length *= 2;
		Temp = (VarString_Typedef)malloc(SArray->pArray_Length * sizeof(String_Typedef*));
		memcpy(Temp, SArray->pArray, (SArray->pArray_Length / 2) * sizeof(String_Typedef*));
		free(SArray->pArray);
		SArray->pArray = Temp;
	}
	int i = 0;
	for (; i < SArray->Array_Num; i++)
	{
		if (!strcmp(Str, *(SArray->pArray + i)))
		{
			break;
		}
	}
	if (i == SArray->Array_Num)
	{
		*(SArray->pArray + SArray->Array_Num) = (String_Typedef*)malloc((strlen(Str) + 1) * sizeof(String_Typedef));
		memcpy(*(SArray->pArray + SArray->Array_Num), Str, strlen(Str) + 1);
		*(*(SArray->pArray + SArray->Array_Num) + strlen(Str)) = 0;
		SArray->Array_Num++;
	}
}
void StringArray_Destory(StringArray* SArray)
{
	int i = 0;
	for (i = 0;i<SArray->Array_Num;i++)
	{
		free(*(SArray->pArray + i));
	}
	free(SArray->pArray);
	SArray->Array_Num = 0;
	SArray->pArray_Length = 0;
}