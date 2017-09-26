#include "function.h"
#include <pcap.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int shmid = 0;
char* eth_Name = 0;
void getPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    sem_wait(&(Share.Q_state));
    Q_Push(&(Share.Msg), (void *)packet);
    sem_post(&(Share.Contains));
    sem_post(&(Share.Q_state));
}
void Main_Process()
{
    int status;
    u_char i = 0;
    pthread_t tid = 0;
    unsigned char *packet;
    char errbuff[PCAP_ERRBUF_SIZE];
    extern void *pShm;
    pcap_t *device = pcap_open_live(eth_Name, 8000, 1, 0, errbuff);
    if (!device)
    {
        printf("error: pcap_open_live(): %s\n", errbuff);
        exit(1);
    }
    if (pcap_set_rfmon(device, 1) == 0)
    {
        printf("can't enter rfmode\n");
        exit(1);
    }
    get_mac();
    PT_init(&Share);
    Init_thread_Share();
    if ((pShm = shmat(shmid, 0, 0)) <= 0)
    {
        printf("Monitoring Process Failed!\r\n");
        exit(1);
    }
    pthread_create(&tid, NULL, Analysis_Data, NULL);
    pthread_create(&tid, NULL, Analysis_Data, NULL);
    pthread_create(&tid, NULL, Timer, NULL);
    pthread_create(&tid, NULL, Sending_To_Server, NULL);
    //printf("Scaning starts\n");
    pcap_loop(device, -1, getPacket, &i); //Scaning starts
    pcap_close(device);
}
void Create_Monitor_Process()
{
#define MemBase Share_Mem
#define _MemFlag *((char *)MemBase)
#define _Belonging *((char *)MemBase + 1)
#define _Program_Exit *((char *)MemBase + 2)

    pid_t sub_Process = 0;
    void *Share_Mem = NULL;
    sub_Process = fork();
    if (sub_Process == 0) //child process
    {
        if ((Share_Mem = shmat(shmid, 0, 0)) <= 0)
        {
            printf("Monitoring Process Failed!\r\n");
            exit(1);
        }
        while (1)
        {
            sleep_m(5, 0);
            if (_Program_Exit == Program_Exit)
            {
                printf("Monition Thread Exit.Stop Scanning.\r\n");
                exit(1);
            }
            if( _MemFlag <= -1)
            {
                printf("Main Process is Dead.\r\n");
                sub_Process = fork();
                if(_Belonging == Montion_Create_Main)
                {
                    wait(NULL);
                }
                else
                {
                    _Belonging = Montion_Create_Main; //Monitor Create Main
                }
                if (sub_Process == 0) //child process(main)
                {
                    break;
                }
            }
            else
            {
                _MemFlag = _MemFlag - 1;
            }
        }
        Main_Process(eth_Name);
    }
    else if (sub_Process < 0)
    {
        exit(1);
    }
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage:./name <wlan name>\n");
        exit(1);
    }
    if (fork() == 0)
    {
        void *Share_Mem = NULL;
        if ((shmid = shmget(0, 4, 0666 | IPC_CREAT)) == -1)
        {
            printf("Share_Mem_Create_Failed!\r\n");
            exit(1);
        }
        if ((Share_Mem = shmat(shmid, 0, 0)) <= 0)
        {
            printf("Monitoring Process Failed!\r\n");
            exit(1);
        }
        *((char *)Share_Mem + 1) = Main_Create_Montion;//Main Create Monitor
        *((char *)Share_Mem + 2) = Program_Running;//Running
        eth_Name = (char *)malloc(strlen(argv[1]) + 1);
        memcpy(eth_Name, argv[1], strlen(argv[1]) + 1);
        Create_Monitor_Process();
        Main_Process();
    }
    return 0;
}
