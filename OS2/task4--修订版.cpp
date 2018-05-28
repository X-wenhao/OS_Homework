#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Param
{
	char name[20];
}PARAM;

HANDLE full,empty,mutex;
int num[5]={0};

DWORD WINAPI produce(LPVOID p)
{
	PARAM *pa=(PARAM *)p;
	
	while(1)
	{
		WaitForSingleObject(empty,INFINITE);
		WaitForSingleObject(mutex,INFINITE);
		int i=0;
		for(i=0;i<5;i++)
		{
			if(num[i]==0) break;
		}
		num[i]=rand();
		printf("%s procudes product%d(%d)!\n",pa->name,i,num[i]);
		Sleep(500);
		ReleaseMutex(mutex);
		ReleaseSemaphore(full,1,NULL);		
		
	}
	

	return 0;
}

DWORD WINAPI consume(LPVOID p)
{
	PARAM *pa=(PARAM *)p;
	while(1)
	{
		WaitForSingleObject(full,INFINITE);
		WaitForSingleObject(mutex,INFINITE);
		int i=0;
		for (i=0;i<5;i++)
		{
			if (num[i]!=0) break;
		}
		printf("%s consumes product%d(%d)!\n",pa->name,i,num[i]);
		num[i]=0;
		Sleep(1000);
		ReleaseMutex(mutex);
		ReleaseSemaphore(empty,1,NULL);	
		
		
	}
	return 0;
}


int main()
{	
	full = CreateSemaphore(NULL,0,5,"FULL");
	empty = CreateSemaphore(NULL,5,5,"EMPTY");
	mutex = CreateMutex(NULL,FALSE,"MUTEX");
	
	PARAM data[6]={{"Producer1"},{"Producer2"},{"Consumer1"},{"Consumer2"},{"Consumer3"},{"Consumer4"}};
	
	HANDLE hThread[6];
	hThread[0]=CreateThread(NULL,0,produce,&data[0],0,NULL);
	Sleep(2000);
	hThread[1]=CreateThread(NULL,0,produce,&data[1],0,NULL);
	hThread[2]=CreateThread(NULL,0,consume,&data[2],0,NULL);
	hThread[3]=CreateThread(NULL,0,consume,&data[3],0,NULL);	
	hThread[4]=CreateThread(NULL,0,consume,&data[4],0,NULL);
	hThread[5]=CreateThread(NULL,0,consume,&data[5],0,NULL);	
	WaitForMultipleObjects(4,hThread,TRUE,INFINITE);
	
	CloseHandle(full);
	CloseHandle(empty);
	CloseHandle(mutex);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);
	CloseHandle(hThread[3]);	
	
	
	return 0;
	
}

