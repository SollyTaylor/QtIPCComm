#ifndef IPC_TEST_H
#define IPC_TEST_H

#include <QtWidgets/QMainWindow>
#include<QSharedMemory>
#include<QFileDialog>
#include<QBuffer>
#include<QDebug>
#include<QDateTime>
#include <QTimer>
#include "ui_ipc_test.h"
#include <atlstr.h>
//#include "global.h"
//class QSharedMemory;

class IPC_test : public QMainWindow
{
	Q_OBJECT

public:
	IPC_test(QWidget *parent = 0);
	~IPC_test();

private:
	Ui::IPC_testClass ui;
	QSharedMemory *sharedMemory;
	QString filename;
	CString fileName;
	double byets;
	QString showText;
	HANDLE hServerWritten;
	HANDLE hclintWritten;
	QTimer * timerClient;
	QTimer * timerServer;
	QTime msTime;
	int timeSinStart;
	int time_Diff;

public slots:
	//void chooseFile();
	void loadFromFile();
	void loadFromMemory();
	void timerUpdateClient();
	void timerUpdateServer();
	void initialize();
};

#endif // IPC_TEST_H
