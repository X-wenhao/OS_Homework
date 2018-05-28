#include<windows.h>
#include <stdlib.h>
#include <time.h>
#include<stdio.h>
#include <graphics.h>  
#include <math.h>
#define PI 3.141592653
#define EATING RED
#define THINKING GREEN
#define USING BLACK
#define UNUSING GREEN
#define WAITUSING BLUE
#define IS_DEADLOCK 0    //�Ƿ�����
HANDLE hMutex[5];   //�����ٽ���
HANDLE p[5];

void init();
void draw(int id, int status, int type);
DWORD  WINAPI philosopher(LPVOID lpParam);

int main()
{
	init();
	srand((unsigned int)100);
	for (int i = 0;i<5;i++)
	{
		hMutex[i] = CreateMutex(NULL, FALSE, NULL);   //��ʼ���ٽ���
	}

	DWORD ThreadID;
	int id[5];
	for (int i = 0;i< 5;i++)
	{
		id[i] = i;
		p[i]= CreateThread(NULL, 0, philosopher, &id[i], 0, &ThreadID);  //�����ӽ���
	}
	//Sleep(3000000);
	WaitForMultipleObjects(5, p, TRUE, INFINITE);
	return 0;
}

/*
 *��ѧ�Һ���������Ϊ��ѧ��id
 */
DWORD  WINAPI philosopher(LPVOID lpParam)
{
	int id = *((int*)lpParam);
	int a[2] = { id ,(id + 4) % 5 };
	if (!IS_DEADLOCK)    //�����Ƿ�����ȷ����Դ��ȡ�Ⱥ�
	{
		if (a[0]>a[1])
		{
			int tmp = a[0];
			a[0] = a[1];
			a[1] = tmp;
		}
	}
	while (TRUE)
	{	
		//�Է�
		WaitForSingleObject(hMutex[a[0]], INFINITE);
		draw(a[0], WAITUSING, 0);
		Sleep(100);                //����ʵ���������
		WaitForSingleObject(hMutex[a[1]], INFINITE);
		draw(a[0], USING, 0);
		draw(a[1], USING, 0);
		draw(id, EATING, 1);
		Sleep(2000 + rand() % 1000);
		draw(id, THINKING, 1);
		draw(a[1], UNUSING, 0);
		ReleaseMutex(hMutex[a[1]]);
		draw(a[0], UNUSING, 0);
		ReleaseMutex(hMutex[a[0]]);	
		//˼��
		Sleep(2000 + rand() % 1000);	
	}
}
/*
 *��ͼ����ѧ��ΪԲ������Ϊ��
 *id:��ѧ��/���ӱ��  status:״̬����ɫ��  type:1->��ѧ�� 0->����
 */
void draw(int id, int status, int type)
{
	
	if (type)
	{
		setfillcolor(status);		
		solidcircle(320 + 90 * cos(2 * PI / 5 * id), 240 + 90 * sin(2 * PI / 5 * id), 30);
	}
	else {
		setcolor(status);
		line(320 + 60 * cos(2 * PI / 5 * id + PI / 5), 240 + 60 * sin(2 * PI / 5 * id + PI / 5),
			320 + 120 * cos(2 * PI / 5 * id + PI / 5), 240 + 120 * sin(2 * PI / 5 * id + PI / 5));
	}
	FlushBatchDraw();
}

/*
 *��ʼ������
*/
void init()
{
	initgraph(640, 480);
;
	BeginBatchDraw();
	for (int i = 0;i < 5;i++)
	{
		draw(i, THINKING, 1);
		draw(i, UNUSING, 0);
	}
}


