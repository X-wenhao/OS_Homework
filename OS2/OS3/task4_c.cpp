#include <windows.h>
#include <stdio.h> 
#include <tchar.h>
int main()
{
	HANDLE stdRead;
	HANDLE stdWrite;  //重定向使用的管道 
	
	SECURITY_ATTRIBUTES saAttr = {0};  
    saAttr.nLength = sizeof(saAttr);  
    saAttr.bInheritHandle = TRUE;    
	if(!CreatePipe(&stdRead,&stdWrite,&saAttr,NULL)) return 0; //创建管道 
	
	STARTUPINFO aStartInfo; 
	STARTUPINFO bStartInfo;    //子进程信息 
	
	ZeroMemory( &aStartInfo, sizeof(STARTUPINFO) );  
  	aStartInfo.cb = sizeof(STARTUPINFO);  
  	aStartInfo.dwFlags  |= STARTF_USESHOWWINDOW;  
  	aStartInfo.dwFlags  |= STARTF_USESTDHANDLES;  //使子进程管道可重定向 
  	aStartInfo.hStdOutput = stdWrite;   //子进程A的标准输出重定向到管道write端 
  	
  	ZeroMemory( &bStartInfo, sizeof(STARTUPINFO) );  
  	bStartInfo.cb = sizeof(STARTUPINFO);  
  	bStartInfo.dwFlags  |= STARTF_USESHOWWINDOW;  
  	bStartInfo.dwFlags  |= STARTF_USESTDHANDLES;  
  	bStartInfo.hStdInput = stdRead;
  	
  	PROCESS_INFORMATION pi1;
  	PROCESS_INFORMATION pi2;
  	ZeroMemory(&pi1, sizeof(pi1));
  	ZeroMemory(&pi2, sizeof(pi2));
	  
	if(!CreateProcess(NULL,
		"C:\\Users\\夏文浩\\Desktop\\OS3\\task4_a.exe",
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&aStartInfo,
		&pi1
	)) return 1;   
	CreateProcess(NULL,
		"C:\\Users\\夏文浩\\Desktop\\OS3\\task4_b.exe",
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&bStartInfo,
		&pi2
	);
	
	WaitForSingleObject(pi1.hProcess, INFINITE);
	WaitForSingleObject(pi2.hProcess, INFINITE);
	
}
