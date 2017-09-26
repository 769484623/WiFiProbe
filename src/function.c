#include "function.h"
#include "protocol_pack.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <stdlib.h>

static Queue Anal_Sending;
static sem_t Q_state;
static List Device_List;

void *pShm = NULL;

#define MemBase pShm
#define _MemFlag *((char *)MemBase)
#define _Belonging *((char *)MemBase + 1)
#define _Program_Exit *((char *)MemBase + 2)

static char print_radiotap_namespace(struct ieee80211_radiotap_iterator *iter, Packet_Info *Info)
{
	uint32_t phy_freq = 0;
	switch (iter->this_arg_index)
	{
	// 通信信息
	case IEEE80211_RADIOTAP_CHANNEL:
		Info->Phy_Freq = le16toh(*(uint16_t *)iter->this_arg);
		break;
	// 信号强度
	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
		Info->RSSI = *(signed char *)iter->this_arg;
		if (Info->RSSI >= 0)
		{
			return -1;
		}
		break;
	case IEEE80211_RADIOTAP_RATE:
	//printf("\trate: %.2f Mbit/s\n", (double)*iter->this_arg / 2);
	case IEEE80211_RADIOTAP_TSFT:
	case IEEE80211_RADIOTAP_FLAGS:
	case IEEE80211_RADIOTAP_RX_FLAGS:
	case IEEE80211_RADIOTAP_ANTENNA:
	case IEEE80211_RADIOTAP_RTS_RETRIES:
	case IEEE80211_RADIOTAP_DATA_RETRIES:
	case IEEE80211_RADIOTAP_FHSS:
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
	case IEEE80211_RADIOTAP_TX_FLAGS:
		break;
	default:
		return -1;
	}
	return 0;
}

void sleep_m(int sec, int micro)
{
	struct timeval t_timeval;
	t_timeval.tv_sec = sec;
	t_timeval.tv_usec = micro * 1000;
	select(0, NULL, NULL, NULL, &t_timeval);
}
void PT_init(PT *p)
{
	Q_Init(&(p->Msg));
	if (sem_init(&(p->Q_state), 0, 1) || sem_init(&(p->Contains), 0, 0))
	{
		printf("SEM INIT FAILED!\n");
		exit(1);
	}
}
void Init_thread_Share()
{
	Q_Init(&Anal_Sending);
	List_Init(&Device_List);
	sem_init(&Q_state, 0, 1);
}
void Command_Exe(const char *str)
{
	char *Index = str;
	Index = strstr(str, "\r\n\r\n") + 4;
	if (Index)
	{
		char* pStr = NULL;
		if (pStr = strstr(Index, "reboot"))
		{
			if( *(pStr + strlen("reboot") + 2) == '1')
			{
				printf("reboot\r\n");
				system("reboot");
				exit(0);
			}
		}
		/*
		if(strstr(Index,"update"))
		{

		}
		*/
		printf("%s\r\n", Index);
	}
}
void *Timer(void *p)
{
	int Flag = 1;
	char Counter = 0;
	char *Command = (char *)malloc(35);
	while (1)
	{
		if (_Program_Exit == Program_Exit)
		{
			printf("Main Thread Exit.\r\n");
			exit(1);
		}
		if (_MemFlag == 1)
		{
			Counter++;
			if (Counter >= 40)
			{
				Counter = 0;
				printf("Monitor Dead\r\n");
				if (_Belonging == Main_Create_Montion)
				{
					wait(NULL);
				}
				else
				{
					_Belonging = Main_Create_Montion; //Main Create Monitor
				}
				Create_Monitor_Process();
			}
		}
		else
		{
			_MemFlag = 1;
			Counter = 0;
		}
		memset(Command, 0, 35);
		sprintf(Command, "iw wlan0 set channel %d", Flag++);
		system((const char *)Command); //更改信道
		if (Flag > 11)
		{
			Flag = 1;
		}
		sleep_m(0, 250);
	}
}
void *Analysis_Data(void *p)
{
	int i = 0;
	i3e_header *header = 0;
	while (1)
	{
		sem_wait(&(Share.Contains)); //if empty
		sem_wait(&(Share.Q_state));  //if busy
		const u_char *packet = (const u_char *)Q_Pop(&(Share.Msg));
		sem_post(&(Share.Q_state));
		if (packet == 0)
		{
			continue;
		}
		struct ieee80211_radiotap_iterator iter;
		if (ieee80211_radiotap_iterator_init(&iter, (struct ieee80211_radiotap_header *)packet, ((struct ieee80211_radiotap_header *)packet)->it_len, NULL))
		{
			continue;
		}
		header = (i3e_header *)(packet + le16toh(((struct ieee80211_radiotap_header *)packet)->it_len));
		header->fc = le16toh(header->fc);
		u_char type = (header->fc & 0x0c) >> 2;
		u_char stype = (header->fc & 0xf0) >> 4;
		Packet_Info Info;
		Packet_Info_Init(&Info);
		switch (type)
		{
		case 0x00:
		{
			if (stype == 0x04 || stype == 0x05 || stype == 0x08) //probe request /resp
			{
				struct ControlFrame_Body Body;
				memcpy(&Body, (uint8_t *)header + 24, sizeof(struct ControlFrame_Body));
				Body.SSID = (uint8_t *)header + 38;
				if (Body.Element_ID == 0)
				{
					Body.SSID_Length > 32 ? Body.SSID_Length = 32 : Body.SSID_Length;
					char *ESSID = (char *)malloc(Body.SSID_Length + 1);
					memcpy(ESSID, Body.SSID, Body.SSID_Length);
					ESSID[Body.SSID_Length] = 0;
					Info.ESSID = ESSID;
				}
			}
		}
		case 0x02:
		{
			memcpy(Info.Source_Mac.MAC_SLICE, header->sa, 6);
			memcpy(Info.Target_Station_Mac.MAC_SLICE, header->da, 6);
			break;
		}
		default:
		{
			continue;
		}
		}
		if (Info.Source_Mac.MAC_SLICE[0] == 0x00)
		{
			if (Info.ESSID)
			{
				free(Info.ESSID);
			}
			continue;
		}
		int j = 0;
		while (!ieee80211_radiotap_iterator_next(&iter))
		{
			if (iter.is_radiotap_ns)
			{
				if (print_radiotap_namespace(&iter, &Info) == -1)
				{
					j = -1;
					break;
				}
			}
			j++;
		}
		if (j == -1)
		{
			if (Info.ESSID)
			{
				free(Info.ESSID);
			}
			continue;
		}
		Info.PowerMange = header->fc >> 12 & 0x01;
		sem_wait(&Q_state);
		List_Add(&Device_List, &Info);
		sem_post(&Q_state);
		if (Info.ESSID)
		{
			free(Info.ESSID);
		}
	}
	pthread_exit(NULL);
}
void *Sending_To_Server(void *p)
{
	int sockfd;
	struct sockaddr_in servaddr;
	pthread_t tid = 0;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;									   //ipv4
	servaddr.sin_port = htons(4100);								   //8000 port
	if (inet_pton(AF_INET, "123.206.84.193", &servaddr.sin_addr) <= 0) //ip
	{
		exit(1);
	}
	Init_thread_Share();
	while (1)
	{
		char *Msg_Word = 0;
		sleep_m(3, 0);
		sem_wait(&Q_state);
		List_Flip(&Device_List);
		List_Show(&Device_List);
		Msg_Word = List_Element_To_Str(&Device_List);
		sem_post(&Q_state);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
			continue;
		}
		if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) //can not connet to server
		{
			printf("Cannot connect Server!\r\n");
		}
		else
		{
			char Rec[4097] = {0};
			if (send(sockfd, Msg_Word, strlen(Msg_Word), 0) < 0)
			{
				break;
			}
			if (recv(sockfd, Rec, 4096, 0) > 0)
			{
				Command_Exe(Rec);
			}
			printf("=============================================================\r\n");
		}
		close(sockfd);
		if (Msg_Word)
		{
			free(Msg_Word);
		}
	}
	pthread_exit(NULL);
}
