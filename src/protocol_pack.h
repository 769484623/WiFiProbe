#include <time.h>
#include <string.h>
#include <malloc.h>
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>  
#include <unistd.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "radiotap_iter.h"
typedef struct Receive_packC
{
	char Beacon[4];
	union
	{
		char Data[60];
	};
}Client_RPack;
extern char* Pack(const char* MAC,uint32_t Num);
extern int get_mac();
