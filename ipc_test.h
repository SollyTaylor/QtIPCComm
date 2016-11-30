#ifndef IPC_TEST_H
#define IPC_TEST_H

#include <QtWidgets/QMainWindow>
#include<QSharedMemory>
#include<QFileDialog>
#include<QBuffer>
#include<QDebug>
#include<QDateTime>
#include "ui_ipc_test.h"

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
	double byets;
	QString showText;
public slots:
	void chooseFile();
	void loadFromFile();
	void loadFromMemory();
};

#endif // IPC_TEST_H
