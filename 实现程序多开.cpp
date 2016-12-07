#include <windows.h>
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>

#define CLINTNUM 50

using namespace std;

void creatIPCProcess()
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));//初始化si在内存块中的值（详见memset函数）
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;//必备参数设置结束
	if (!CreateProcess(TEXT("..\\Win32\\Release\\IPC_test.exe"),
		NULL,
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
	for (int i = 0; i < CLINTNUM; i++)
	{
		//cout<<ShellExecute(0, (LPCSTR)L"open", softPath, (LPCSTR)L"", (LPCSTR)L"", SW_SHOWNORMAL)<<endl;
		//system(softPath);
		cout << "the " << i+1 << " th process is open with ";
		creatIPCProcess();
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