#ifndef Device_Stat
#define Device_Stat

#include "VarArray.h"

typedef union{
	long long int MAC_STORE;
	unsigned char MAC_SLICE[8];
}MAC_Def;
typedef struct _Node
{
    MAC_Def Key;
    MAC_Def Target_Station_Mac;
    unsigned char Status;
    unsigned char PowerMange;
	unsigned int Phy_Freq;
    StringArray ESSID;
    struct _Node* Next;
	int RSSI;
    int RSSI_Counter;
}Node;
typedef struct
{
    MAC_Def Source_Mac;
    MAC_Def Target_Station_Mac;
    unsigned char PowerMange;
	unsigned int Phy_Freq;
    char* ESSID;
    char RSSI;
}Packet_Info;
void Packet_Info_Init(Packet_Info* Info);
typedef struct
{
    int Num;
    Node* Header;
}List;
void List_Init(List* pList);
void List_Destory(List* pList);
void List_Add(List* pList,Packet_Info* Info);
void List_Flip(List *pList);
char* List_Element_To_Str(List *pList);
void List_Show(List *pList);
#endif