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
	memset(&si, 0, sizeof(STARTUPINFO));//��ʼ��si���ڴ���е�ֵ�����memset������
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;//�ر��������ý���
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
	//��ʹ�õľ����ùص�
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}


int main(void)
{
	char* softPath = "..\\Win32\\Release\\IPC_test.exe";
	char* testPath = "..\\Win32\\Release\\test.txt";
	int serverMode,clientMode,clientNum;
	cout << "���������ͣ�IPC_SERVER_PUB 4"<<endl;
	cout << "������Ҫ�����ķ���������:" << endl;
	cin >> serverMode;
	creatIPCProcess(serverMode);//�������������
	cout << "�ͻ�������:IPC_CLINT_SUB 3" << endl;
	cout << "���봴���Ŀͻ�������" << endl;
	cin >> clientMode;
	cout << "����ͻ�������" << endl;
	cin >> clientNum;
	for (int i = 0; i < clientNum; i++)//����CLINTNUM�������
	{
		//cout<<ShellExecute(0, (LPCSTR)L"open", softPath, (LPCSTR)L"", (LPCSTR)L"", SW_SHOWNORMAL)<<endl;
		//system(softPath);
		cout << "the " << i+1 << " th process is open with ";
		creatIPCProcess(clientMode);
	}
	cout << "���������ַ������س��˳�" << endl;
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