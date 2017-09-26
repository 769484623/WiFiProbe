#include"Queue.h"
#include<string.h>
#include<malloc.h>
void Q_Init(Queue *p)
{
	int i;
	p->Empty=1;
	p->Full=0;
	p->Front=&(p->fifo[0]);
	p->Back=&(p->fifo[0]);
	for(i=0;i<=MAX_ELENMENT-1;i++)
	{
		memset(&(p->fifo[i]), 0, 4);
	}
}
char Q_Is_Empty(Queue*p)
{
	if((p->Empty)&&(p->Back==p->Front))
	{
		return 1;
	}
	p->Empty=0;
	return 0;
}
char Q_Push(Queue *p,void *k)
{
	p->Empty=0;
	if(p->Full)
	{
		return 0;
	}
	else
	{
		p->fifo[p->Front-p->fifo]=k;
		if(p->Front-p->fifo==MAX_ELENMENT-1)
		{
			p->Front=p->fifo;
		}
		else
		{
			p->Front++;
		}
		if(p->Front==p->Back)
		{
			p->Full=1;
		}
		return 1;
	}
}
void* Q_Pop(Queue *p)
{
	void* Temp;
	p->Full=0;
	if(p->Empty)
	{
		return 0;
	}
	else
	{
		if(p->Back-p->fifo==MAX_ELENMENT-1)
		{
			p->Back=p->fifo;
			Temp=p->fifo[MAX_ELENMENT-1];
			p->fifo[MAX_ELENMENT-1]=0;
			return Temp;
		}
		else
		{
			Temp=p->fifo[p->Back-p->fifo];
			p->fifo[p->Back-p->fifo]=0;	
			p->Back++;
			if(p->Back==p->Front)
			{
				p->Empty=1;
			}
			return Temp;
		}
	}
}
void Q_Destory(Queue* p)
{
	int i;
	for(i=0;i<MAX_ELENMENT-1;i++)
	{
		free(p->fifo[i]);
		p->fifo[i]=0;
	}
}
