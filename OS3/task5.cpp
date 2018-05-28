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
#define IS_DEADLOCK 0    //是否死锁
HANDLE hMutex[5];   //筷子临界区
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
		hMutex[i] = CreateMutex(NULL, FALSE, NULL);   //初始化临界区
	}

	DWORD ThreadID;
	int id[5];
	for (int i = 0;i< 5;i++)
	{
		id[i] = i;
		p[i]= CreateThread(NULL, 0, philosopher, &id[i], 0, &ThreadID);  //创建子进程
	}
	//Sleep(3000000);
	WaitForMultipleObjects(5, p, TRUE, INFINITE);
	return 0;
}

/*
 *哲学家函数，参数为哲学家id
 */
DWORD  WINAPI philosopher(LPVOID lpParam)
{
	int id = *((int*)lpParam);
	int a[2] = { id ,(id + 4) % 5 };
	if (!IS_DEADLOCK)    //根据是否死锁确定资源获取先后
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
		//吃饭
		WaitForSingleObject(hMutex[a[0]], INFINITE);
		draw(a[0], WAITUSING, 0);
		Sleep(100);                //便于实现死锁情况
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
		//思考
		Sleep(2000 + rand() % 1000);	
	}
}
/*
 *画图，哲学家为圆，筷子为线
 *id:哲学家/筷子编号  status:状态（颜色）  type:1->哲学家 0->筷子
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
 *初始化界面
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


