#include "device_stat.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
void Packet_Info_Init(Packet_Info *Info)
{
    memset(Info, 0, sizeof(Packet_Info));
}
void List_Init(List *pList)
{
    pList->Header = 0;
    pList->Num = 0;
}
void List_Destory(List *pList)
{
    Node *pCurrent = pList->Header;
    while (pCurrent)
    {
        Node *Temp = pCurrent;
        pCurrent = pCurrent->Next;
        free(Temp);
    }
}
void List_Node_Init(Node *pNode)
{
    pNode->Status = 1;
    pNode->Next = 0;
    pNode->Key.MAC_STORE = 0;
    pNode->RSSI = 0;
    pNode->RSSI_Counter = 0;
    StringArray_Init(&(pNode->ESSID));
}
void Node_WriteInfo(Node *pNode, Packet_Info *Info)
{
    if (Info->RSSI >= 0)
    {
        return;
    }
    pNode->Phy_Freq = Info->Phy_Freq;
    pNode->PowerMange = Info->PowerMange;
    pNode->Key.MAC_STORE = Info->Source_Mac.MAC_STORE;
    pNode->Target_Station_Mac.MAC_STORE = Info->Target_Station_Mac.MAC_STORE;
    if (pNode->RSSI_Counter > 65536)
    {
        pNode->RSSI_Counter = 1;
        pNode->RSSI = Info->RSSI;
    }
    else
    {
        pNode->RSSI += Info->RSSI;
        pNode->RSSI_Counter++;
    }
    pNode->Status = 1;
    StringArray_AddString(&(pNode->ESSID), Info->ESSID);
}
void List_Add(List *pList, Packet_Info *Info)
{
    MAC_Def sMAC;
    memcpy(sMAC.MAC_SLICE, Info->Source_Mac.MAC_SLICE, 6);
    Node *pCurrent = pList->Header;
    if (pList->Header == 0)
    {
        Node *pNode = (Node *)malloc(sizeof(Node));
        List_Node_Init(pNode);
        Node_WriteInfo(pNode, Info);
        pList->Header = pNode;
        pList->Num++;
    }
    else
    {
        if (pCurrent->Key.MAC_SLICE[0] == sMAC.MAC_SLICE[0] && pCurrent->Key.MAC_SLICE[1] == sMAC.MAC_SLICE[1] && pCurrent->Key.MAC_SLICE[2] == sMAC.MAC_SLICE[2] && pCurrent->Key.MAC_SLICE[3] == sMAC.MAC_SLICE[3] && pCurrent->Key.MAC_SLICE[4] == sMAC.MAC_SLICE[4] && pCurrent->Key.MAC_SLICE[5] == sMAC.MAC_SLICE[5])
        {
            Node_WriteInfo(pCurrent, Info);
        }
        else
        {
            while (pCurrent->Next)
            {
                if (pCurrent->Key.MAC_SLICE[0] == sMAC.MAC_SLICE[0] && pCurrent->Key.MAC_SLICE[1] == sMAC.MAC_SLICE[1] && pCurrent->Key.MAC_SLICE[2] == sMAC.MAC_SLICE[2] && pCurrent->Key.MAC_SLICE[3] == sMAC.MAC_SLICE[3] && pCurrent->Key.MAC_SLICE[4] == sMAC.MAC_SLICE[4] && pCurrent->Key.MAC_SLICE[5] == sMAC.MAC_SLICE[5])
                {
                    Node_WriteInfo(pCurrent, Info);
                    break;
                }
                pCurrent = pCurrent->Next;
            }
            if (!(pCurrent->Next))
            {
                if (pCurrent->Key.MAC_SLICE[0] == sMAC.MAC_SLICE[0] && pCurrent->Key.MAC_SLICE[1] == sMAC.MAC_SLICE[1] && pCurrent->Key.MAC_SLICE[2] == sMAC.MAC_SLICE[2] && pCurrent->Key.MAC_SLICE[3] == sMAC.MAC_SLICE[3] && pCurrent->Key.MAC_SLICE[4] == sMAC.MAC_SLICE[4] && pCurrent->Key.MAC_SLICE[5] == sMAC.MAC_SLICE[5])
                {
                    Node_WriteInfo(pCurrent, Info);
                }
                else
                {
                    Node *pNode = (Node *)malloc(sizeof(Node));
                    List_Node_Init(pNode);
                    Node_WriteInfo(pNode, Info);
                    pCurrent->Next = pNode;
                    pList->Num++;
                }
            }
        }
    }
}
void Show_Node(Node *pNode)
{

    unsigned char i = 0;
    printf("deleting:\r\n");
    for (i = 0; i < 6; ++i)
    {
        printf("%02x ", pNode->Key.MAC_SLICE[i]);
    }
    printf("\r\nHas ESSID:");
    for (i = 0; i < pNode->ESSID.Array_Num; i++)
    {
        printf("%s", pNode->ESSID.pArray[i]);
    }
    printf("\r\n");
    printf("RSSI_Counter:%d,RSSI_Average:%d\r\n", pNode->RSSI_Counter, pNode->RSSI / pNode->RSSI_Counter);
    printf("Phy_Fre:%d MHz,PowerManage:%d\r\n", pNode->Phy_Freq, pNode->PowerMange);
}
void List_Flip(List *pList)
{
    Node *pCurrent = pList->Header;
    Node *pLast = 0;
    while (pCurrent)
    {
        if (pCurrent->Status)
        {
            pCurrent->Status = 0;
            pLast = pCurrent;
            pCurrent = pCurrent->Next;
        }
        else
        {
            Node *Temp = pCurrent;
            pCurrent = pCurrent->Next;
            if (pLast == 0 && pCurrent == 0)
            {
                pList->Header = 0;
            }
            else
            {
                if (pLast == 0)
                {
                    pList->Header = pCurrent;
                }
                else
                {
                    pLast->Next = pCurrent;
                }
            }
            //Show_Node(Temp);
            StringArray_Destory(&(Temp->ESSID));
            free(Temp);
            (pList->Num)--;
        }
    }
}
void List_Show(List *pList)
{
    Node *pCurrent = pList->Header;
    printf("Num is %d\r\n", pList->Num);
    /*
    while (pCurrent)
    {
        printf("%x:%x:%x:%x:%x:%x   ",
        pCurrent->Key.MAC_SLICE[0],pCurrent->Key.MAC_SLICE[1],pCurrent->Key.MAC_SLICE[2],
        pCurrent->Key.MAC_SLICE[3],pCurrent->Key.MAC_SLICE[4],pCurrent->Key.MAC_SLICE[5]
        );
        printf("Status:%d\r\n",pCurrent->Status);
        pCurrent = pCurrent->Next;
    }
    printf("End.\r\n");
    */
}
char *List_Element_To_Str(List *pList)
{
    extern unsigned char mac_addr[6];
    int Str_Length = 0;
    char Flag = 0;
    Node *pCurrent = pList->Header;
    /***************Fill the Request Body***************/
    char *pBody = (char *)malloc(sizeof(char) * pList->Num * 45 + 45);
    Str_Length = sprintf(pBody, "{\"node\":\"%x:%x:%x:%x:%x:%x\",\"nearby\":[", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    while (pCurrent)
    {
        Str_Length += sprintf(pBody + Str_Length, "{\"mac\":\"%x:%x:%x:%x:%x:%x\",\"rssi\":%d},",
                              pCurrent->Key.MAC_SLICE[0], pCurrent->Key.MAC_SLICE[1], pCurrent->Key.MAC_SLICE[2],
                              pCurrent->Key.MAC_SLICE[3], pCurrent->Key.MAC_SLICE[4], pCurrent->Key.MAC_SLICE[5],
                              pCurrent->RSSI / pCurrent->RSSI_Counter);
        pCurrent = pCurrent->Next;
        Flag = 1;
    }
    pBody[Flag ? Str_Length - 1 : Str_Length] = ']';
    pBody[Str_Length++] = '}';
    pBody[Str_Length] = 0;
    /***************Here comes to add Header*************/
    const char *HttpHeader = "POST /mac/post HTTP/1.1\r\nHost: qcloud.zhangzaizai.com\r\nContent-Type: text/plain\r\nAccept: */*\nAccept-Encoding: deflate, br\r\nConnection: close\r\nContent-Length:";
    int pBody_Length = strlen(pBody), pBody_Request_Length = 1;
    for (; pBody_Length /= 10; pBody_Request_Length++)
        ; //Get the length of i to string
    int pStr_Length = strlen(pBody) + strlen(HttpHeader) + pBody_Request_Length + 4;
    char *pStr = (char *)malloc(pStr_Length + 1);
    Str_Length = sprintf(pStr, "%s%d\r\n\r\n%s", HttpHeader, Str_Length, pBody);
    pStr[Str_Length] = 0;
    free(pBody);
    if (Str_Length != pStr_Length)
    {
        printf("Str_Length:%d,pStr_Length:%d,pList_Num:%d\r\n", Str_Length, pStr_Length, pBody_Request_Length);
        printf("%s\r\n", pStr);
        exit(1);
    }
    return pStr;
}