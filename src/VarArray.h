#ifndef _VarArray
#define _VarArray

typedef char String_Typedef;
typedef  String_Typedef** VarString_Typedef;
typedef struct VarStringArray
{
	int Array_Num;
	int pArray_Length;
	VarString_Typedef pArray;
}StringArray;
void StringArray_Destory(StringArray* SArray);
void StringArray_AddString(StringArray* SArray, const String_Typedef* Str);
void StringArray_Init(StringArray* SArray);
#endif