#include <windows.h>
#include <process.h>
#include <iostream>
#include <string.h>
//#include<afx.h>
#include <stdio.h>
#include <conio.h>

#define CLINTNUM 1

#define IPC_TEST 0

#define IPC_CLINT_REG 1
#define IPC_SERVER_REP 2
#define IPC_CLINT_SUB 3
#define IPC_SERVER_PUB 4
#define IPC_VENT 5
#define IPC_WORK 6
#define IPC_SINK 7


using namespace std;

void creatIPCProcess(int ipcMode = IPC_TEST,int nameN=0)
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));//初始化si在内存块中的值（详见memset函数）
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;//必备参数设置结束
	string mode;
	char tem[10];
	sprintf_s(tem, " %d", ipcMode);
	mode = tem;
	if (!CreateProcess(TEXT("..\\Win32\\Release\\IPC_test.exe"),
		tem,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
		)){
		cout << "CreateFail!" << endl;
		exit(1);
	}
	else{
		cout << "Success!" << endl;
	}
	//不使用的句柄最好关掉
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}


int main(void)
{
	char* softPath = "..\\Win32\\Release\\IPC_test.exe";
	char* testPath = "..\\Win32\\Release\\test.txt";
	int serverMode,clientMode,clientNum;
	cout << "服务器类型：IPC_SERVER_PUB 4"<<endl;
	cout << "输入需要创建的服务器类型:" << endl;
	cin >> serverMode;
	creatIPCProcess(serverMode);//创建单个服务端
	cout << "客户端类型:IPC_CLINT_SUB 3" << endl;
	cout << "输入创建的客户端类型" << endl;
	cin >> clientMode;
	cout << "输入客户端数量" << endl;
	cin >> clientNum;
	for (int i = 0; i < clientNum; i++)//创建CLINTNUM个服务端
	{
		//cout<<ShellExecute(0, (LPCSTR)L"open", softPath, (LPCSTR)L"", (LPCSTR)L"", SW_SHOWNORMAL)<<endl;
		//system(softPath);
		cout << "the " << i+1 << " th process is open with ";
		creatIPCProcess(clientMode);
	}
	cout << "输入任意字符并按回车退出" << endl;
	int a;
	cin >> a;

	//for (int i = 0; i < CLINTNUM; i++)
	//{

	//	HWND hWnd = ::FindWindow(NULL, "IPC_test");
	//	if (NULL != hWnd) {
	//		::SendMessage(hWnd, WM_CLOSE, 0, 0);
	//	}
	//}
	while (NULL !=::FindWindow(NULL, "IPC_test"))
	{
		::SendMessage(::FindWindow(NULL, "IPC_test"), WM_CLOSE, 0, 0);
	}
	//system("pause");
	return 0;
}