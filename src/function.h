#include "Queue.h"
#include "typedef.h"
#include "device_stat.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <signal.h>
#include "radiotap_iter.h"
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
struct ControlFrame_Body{
	uint8_t Timestamp[8];
	uint8_t Capability_Info[2];
	uint8_t Listen_Interval[2];
	uint8_t Element_ID;
	uint8_t SSID_Length;
	const uint8_t* SSID;
};
typedef struct header {
	uint16_t fc;
	uint16_t duration;
	uint8_t	da[6];
	uint8_t	sa[6];
	uint8_t	bssid[6];
	uint16_t seq_ctrl;
} i3e_header;
typedef struct Passing_To_Thread
{
	unsigned int node;
	Queue Msg;
	sem_t Q_state,Contains;
}PT;
PT Share;
void PT_init(PT* p);
/*Global Share*/
void Init_thread_Share();
void *Analysis_Data(void*);
void *Sending_To_Server(void*);
void *Timer(void *p);
void sleep_m(int sec,int micro);
