#include <windows.h>
#include <stdio.h> 
#include <tchar.h>
int main()
{
	HANDLE stdRead;
	HANDLE stdWrite;  //�ض���ʹ�õĹܵ� 
	
	SECURITY_ATTRIBUTES saAttr = {0};  
    saAttr.nLength = sizeof(saAttr);  
    saAttr.bInheritHandle = TRUE;    
	if(!CreatePipe(&stdRead,&stdWrite,&saAttr,NULL)) return 0; //�����ܵ� 
	
	STARTUPINFO aStartInfo; 
	STARTUPINFO bStartInfo;    //�ӽ�����Ϣ 
	
	ZeroMemory( &aStartInfo, sizeof(STARTUPINFO) );  
  	aStartInfo.cb = sizeof(STARTUPINFO);  
  	aStartInfo.dwFlags  |= STARTF_USESHOWWINDOW;  
  	aStartInfo.dwFlags  |= STARTF_USESTDHANDLES;  //ʹ�ӽ��̹ܵ����ض��� 
  	aStartInfo.hStdOutput = stdWrite;   //�ӽ���A�ı�׼����ض��򵽹ܵ�write�� 
  	
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
		"C:\\Users\\���ĺ�\\Desktop\\OS3\\task4_a.exe",
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
		"C:\\Users\\���ĺ�\\Desktop\\OS3\\task4_b.exe",
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
