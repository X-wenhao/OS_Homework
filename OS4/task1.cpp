/*
 * 模拟页面调度算法
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#define PMAX 32
#define CNUM 1000
#define FIFO 0
#define LRU 1
typedef struct fifoppage
{
	int vpage;
	int time;
}FIFOPPAGE;

float fifo(FIFOPPAGE * ppage,int ppagenum,int *c )
{
	for ( int i = 0 ; i < ppagenum ; i++)
	{
		ppage[i]={-1,-1};
	}
	int counter = 0;
	for ( int i = 0 ; i < CNUM ; i++)
	{
		int index = -1;
		for (int j = 0;j < ppagenum;j++)
		{
			if (ppage[j].vpage == c[i]) {
				index = j;
				break;
			}
		}
		if (index != -1)
		{
			continue;
		}
		index = 0;
		for (int k = 1; k < ppagenum; k++)
		{
			if (ppage[k].time < ppage[index].time)
			{
				index = k;
			}
		}
		counter++;
		ppage[index].time = i;
		ppage[index].vpage = c[i];
	}
	return 1.0- 1.0*counter/CNUM;
}

float lru(FIFOPPAGE * ppage,int ppagenum, int *c)
{
	for (int i = 0; i < ppagenum; i++)
	{
		ppage[i] = { -1,-1 };
	}
	int counter = 0;
	for (int i = 0; i < CNUM; i++)
	{
		int index = -1;
		for (int j = 0;j < ppagenum;j++)
		{
			if (ppage[j].vpage == c[i]) {
				index = j;
				break;
			}
		}
		if (index != -1)
		{
			ppage[index].time++;
			continue;
		}
		index = 0;
		for (int k = 1; k < ppagenum; k++)
		{
			if (ppage[k].time < ppage[index].time)
			{
				index = k;
			}
		}
		counter++;

		ppage[index].time = 1;
		ppage[index].vpage = c[i];
	}

	return 1.0 - 1.0*counter / CNUM;
}

float opt(FIFOPPAGE * ppage,int ppagenum, int *c)
{
	for (int i = 0; i < ppagenum; i++)ppage[i] = { -1,-1 };
	int counter = 0;
	for (int i = 0; i < CNUM; i++)
	{
		int index = -1;
		for (int j = 0;j < ppagenum;j++)
		{
			if (ppage[j].vpage == c[i]) {
				index = j;
				break;
			}
		}
		if (index != -1)continue;
		index = 0;
		int p = 0;
		for (int k = 0; k < ppagenum; k++)
		{
			for (int m = i + 1;m < CNUM;m++)
			{
				if (ppage[k].vpage == c[m]||m==CNUM-1)
				{
					if (p < m)
					{
						index = k;
						p = m;
					}
					break;
				}
			}
		}
		counter++;
		ppage[index].vpage = c[i];
	}
	return 1.0 - 1.0*counter / CNUM;
}


int main()
{
	srand((unsigned)time(NULL));
	int command[CNUM];
	for(int i = 0; i < CNUM ; i++ )
	{
		command[i]=(rand()%320)/10;
	}
	int ppagenum=4;
	float result[3][29];
	FIFOPPAGE * ppage = (FIFOPPAGE *)malloc(sizeof(FIFOPPAGE)*32);
	for (int i =0 ; i < 29 ; i++)
	{
		result[0][i]=fifo(ppage,ppagenum,command);
		result[1][i] = lru(ppage, ppagenum, command);
		result[2][i] = opt(ppage, ppagenum, command);
		ppagenum++;
	}
	free(ppage);
	printf("---------------FIFO---------------\n");
	for (int i =0 ; i < 29 ; i++)
	{
		printf("%f\n",result[0][i]);
	}
	printf("---------------LRU---------------\n");
	for (int i = 0; i < 29; i++)
	{
		printf("%f\n", result[1][i]);
	}
	printf("---------------OPT---------------\n");
	for (int i = 0; i < 29; i++)
	{
		printf("%f\n", result[2][i]);
	}
	getchar();
}


