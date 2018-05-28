#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<Windows.h>
#define PI 3.14

DWORD WINAPI DrawCircle(LPVOID lp);
DWORD WINAPI DrawRectangle(LPVOID lp);
void init();
int main()
{
	init();
	CreateThread(NULL, 0, DrawCircle, NULL, 0, NULL);
	CreateThread(NULL, 0, DrawRectangle, NULL, 0, NULL);
	_getch();
	closegraph();
}
void init()
{
	initgraph(640, 480, 0);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(RED);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 10);
	_getch();
}
DWORD WINAPI DrawCircle(LPVOID lp)
{
	float i = 0;
	while (i < 2*PI)
	{
		i += 0.02;
		arc(100, 100, 300, 300, 0, i);
		Sleep(4);
	}
	return 0;
}
DWORD WINAPI DrawRectangle(LPVOID lp)
{
	int j = 0;
	moveto(400, 100);
	while (j < 200)
	{
		lineto(400, 100 + j);
		j += 2;
		Sleep(3);
	}
	j = 0;
	while (j < 200)
	{
		lineto(400 + j, 300);
		j += 2;
		Sleep(3);
	}
	j = 0;
	while (j < 200)
	{
		lineto(600, 300 - j);
		j += 2;
		Sleep(3);
	}
	j = 0;
	while (j < 200)
	{
		lineto(600 - j, 100);
		j += 2;
		Sleep(3);
	}
	return 0;
}
