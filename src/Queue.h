#define MAX_ELENMENT 64
typedef struct FIFO
{
	unsigned char Empty,Full;
	void **Front,**Back;
	void *fifo[MAX_ELENMENT];
}Queue;
extern void Q_Init(Queue *p);
extern char Q_Is_Empty(Queue*p);
extern char Q_Push(Queue *p,void *k);
extern void* Q_Pop(Queue *p);
extern void Q_Destory(Queue* p);
