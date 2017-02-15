#ifndef IPC_TEST_H
#define IPC_TEST_H

#include <QtWidgets/QMainWindow>
#include<QSharedMemory>
#include<QFileDialog>
#include<QBuffer>
#include<QDebug>
#include<QDateTime>
//#include <QC>
#include <QTimer>
#include "ui_ipc_test.h"
#include <atlstr.h>


#define IPC_TEST 0

#define IPC_CLINT_REG 1
#define IPC_SERVER_REP 2
#define IPC_CLINT_SUB 3
#define IPC_SERVER_PUB 4
#define IPC_VENT 5
#define IPC_WORK 6
#define IPC_SINK 7
//#include "global.h"
//class QSharedMemory;

class IPC_test : public QMainWindow
{
	Q_OBJECT

public:
	IPC_test(int mode=0,QWidget *parent = 0);
	~IPC_test();

private:
	Ui::IPC_testClass ui;
	QSharedMemory *sharedMemory;
	QString filename;
	CString fileName;
	double byets;
	QString showText;

	bool serverFirst;//������Ƿ��һ�η�������
	bool clientRecieve;//���ͻ����ѽ��ܹ�����

	HANDLE hclintAsk;//���пͻ��˷�������

	HANDLE hClintWriting;//���пͻ�������д������
	HANDLE hServerWriting;//���з��������д������

	HANDLE hclintWritten;//���пͻ��˶�ȡ����
	HANDLE hServerWritten;//���з����д������

	HANDLE hclientSub;




	QTimer * timerClient;
	QTimer * timerServer;
	QTimer * pubTimer;
	QTimer * subTimer;
	QTime msTime;
	int timeSinStart;
	int time_Diff;
	int ipcMode;

	int pubClientNum;
	int pubClientRecieved;

public slots:
	//void chooseFile();
	void loadFromFile();
	void loadFromMemory();
	void timerUpdateClient();
	void timerUpdateServer();
	void serverPub();
	void clientSub();
	void initialize();
};

#endif // IPC_TEST_H
