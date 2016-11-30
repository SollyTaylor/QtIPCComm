#include "ipc_test.h"

const char *KEY_SHARED_MEMORY = "Shared";

IPC_test::IPC_test(QWidget *parent)
	: QMainWindow(parent),
	sharedMemory(new QSharedMemory(KEY_SHARED_MEMORY, this))
{
	ui.setupUi(this);

	ui.picLabel->adjustSize();
	ui.picLabel->setGeometry(QRect(100, 0, 600, 60 * 4));  //四倍行距
	ui.picLabel->setWordWrap(true);
	ui.picLabel->setAlignment(Qt::AlignTop);

	byets = 0;
	sharedMemory->setKey("share3");
	//设置运行与否

	//选择图像（作为最基本的数据单元）
	connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
	//加载图片到共享内存
	connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	//从内存加载到显示
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadFromMemory()));

}
void IPC_test::chooseFile()
{
	filename = QFileDialog::getOpenFileName(this);
}

void IPC_test::loadFromFile()
{
	//for (;;)
	//{
		if (sharedMemory->isAttached()) {
			sharedMemory->detach();
		}
		QImage image;
		image.load(filename);

		QBuffer buffer;
		buffer.open(QBuffer::ReadWrite);
		QDataStream out(&buffer);
		out << image;
		int size = buffer.size();
		
		if (!sharedMemory->create(size)) {
			qDebug() << tr("Create Error: ") << sharedMemory->errorString();
		}
		else {
			sharedMemory->lock();
			char *to = static_cast<char *>(sharedMemory->data());
			const char *from = buffer.data().constData();
			memcpy(to, from, qMin(size, sharedMemory->size()));
			sharedMemory->unlock();
		}
	//}
}

void IPC_test::loadFromMemory()
{
	QDateTime startTime = QDateTime::currentDateTime();
	QTime msTime;
	QTime msTime0;
	showText = "start time:" + startTime.toString("yyyy-MM-dd hh:mm:ss ddd")+"\r\n";
	ui.picLabel->setText(showText);
	QBuffer buffer;
	QDataStream in(&buffer);
	QImage image;

	int time_Diff=0;

	if (!sharedMemory->attach())//将shareMemory与该进程绑定使之可以访问shareMemory里的内容  
	{
		qDebug() << tr("can't attach share memory");
	}

	msTime.start();
	//msTime0.start();
	while (time_Diff<1000)
	{
		
		int time_Diff0 = msTime.elapsed();
		sharedMemory->lock();
		buffer.setData(static_cast<const char *>(sharedMemory->constData()), sharedMemory->size());
		buffer.open(QBuffer::ReadWrite);
		//in >> image;
		sharedMemory->unlock();
		sharedMemory->detach();

		if (buffer.size() != 0)
		{
		byets += ((double)buffer.size())/1024;
		//QDateTime currentTime = QDateTime::currentDateTime();
		time_Diff = msTime.elapsed();
		//stringTem0.sprintf("dataTrans = %d ms", time_Diff - time_Diff0);

		//showText += stringTem0;
		//showText += "\r\n";
		}		
		////char *to = static_cast<char *>(sharedMemory->data());
		////const char *from = buffer.data().constData();

		////buffer.open(QBuffer::ReadWrite);
		////char* to = new char[strlen(buffer.data().constData())+1];
		////strcpy(to, buffer.data().constData());

		////const char *from = (const char *)(sharedMemory->data());

		//sharedMemory->lock();
		////int sizeOfS = sharedMemory->size();
		////img->loadFromData(*(QByteArray*)(sharedMemory->constData()));
		////memcpy(&(buffer.data()), from, sharedMemory->size());
		//sharedMemory->unlock();
		//sharedMemory->detach();

		//if (buffer.size() != 0)
		//{
		//	byets += buffer.size();
		//	QDateTime currentTime = QDateTime::currentDateTime();
		//	int time_Diff = msTime.elapsed();

		//	QString stringTem;
		//	stringTem.sprintf("recive %d byets data,use %d ms", byets, time_Diff);
		//	showText += stringTem;
		//	showText += "\r\n";
		//}

		ui.picLabel->setText(showText);
	}
	QString stringTem;
	stringTem.sprintf("recive %.1f MB data,use %d ms", byets, time_Diff);
	showText += stringTem;
	showText += "\r\n";
	ui.picLabel->setText(showText);
}
IPC_test::~IPC_test()
{

}
