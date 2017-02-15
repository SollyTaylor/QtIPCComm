#include "ipc_test.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	//GetCommandLine
	int openMode;
	if (argc>1)
	{
		openMode = atoi(argv[1]);
	}
	else
	{
		openMode = 0;
	}
	QApplication a(argc, argv);
	IPC_test w(openMode);
	w.show();

	return a.exec();
}
