#include <stdio.h>
#include <windows.h>

DWORD WINAPI count1 ( LPVOID lpParam )
{
	for (int i=0;i<1000;i++)
	{
		printf ("%d \n",i+1); 
		Sleep(500);
	}
	return 0;
}

DWORD WINAPI count2 (LPVOID lpParam )
{
	for (int i=1000;i>0;i--)
	{
		printf ("%d \n",i);
		Sleep(500); 
	}
	return 0;
}

int main()
{	
	
	HANDLE hThread[2];
	hThread[0]= CreateThread(NULL,0,count1,NULL,0,NULL);
	hThread[1]= CreateThread(NULL,0,count2,NULL,0,NULL);
	WaitForMultipleObjects(2,hThread,TRUE,INFINITE);
	
	printf("end\n");
}
