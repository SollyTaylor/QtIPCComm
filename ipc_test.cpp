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

	//客户端程序启动时开始计时
	timeSinStart = 0;
	time_Diff = 0;

	//设置客户端检测的定时器
	timerClient = new QTimer(this);
	timerClient->start(3);
	connect(timerClient, SIGNAL(timeout()), this, SLOT(timerUpdateClient()));

	timerServer = new QTimer(this);
	timerServer->start(3);
	connect(timerServer, SIGNAL(timeout()), this, SLOT(timerUpdateServer()));

	//设置运行与否

	//选择图像（作为最基本的数据单元）（更改为自动去test文件夹下找数据图片）
	//connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
	//加载图片到共享内存
	connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	//从内存加载到显示
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadFromMemory()));

	LPCSTR ServerWriteOver = (LPCSTR)"ServerWriteOver";
	LPCSTR ClientWriteOver = (LPCSTR)"ClientWriteOver";
	//LPCSTR ServerWriteOver = (LPCSTR)"ServerWriteOver";
	hServerWritten = CreateEventA(NULL, TRUE, FALSE, ServerWriteOver);
	hclintWritten = CreateEventA(NULL, TRUE, FALSE, ClientWriteOver);
	ResetEvent(hServerWritten);
	ResetEvent(hclintWritten);

	initialize();
}
//void IPC_test::chooseFile()
//{
//	filename = QFileDialog::getOpenFileName(this);
//}
void IPC_test::initialize()
{
	QDateTime startTime = QDateTime::currentDateTime();
	showText = "start time:" + startTime.toString("yyyy-MM-dd hh:mm:ss ddd") + "\r\n";
	ui.picLabel->setText(showText);
	//开始计时
	msTime.start();
}
void IPC_test::loadFromFile()
{
	//for (;;)
	//{
		filename = ".\\testData\\test.jpg";
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
			SetEvent(hServerWritten);
		}

	//}
}

void IPC_test::timerUpdateClient()
{
	if (WaitForSingleObject(hServerWritten, 3) == WAIT_OBJECT_0)
	{
		if (WaitForSingleObject(hclintWritten, 3) != WAIT_OBJECT_0)
		{
			SetEvent(hclintWritten);
			loadFromMemory();
			ResetEvent(hclintWritten);
			ResetEvent(hServerWritten);
		}
	}
}
void IPC_test::timerUpdateServer()
{
	if (WaitForSingleObject(hServerWritten, 3) != WAIT_OBJECT_0)
	{
		if (WaitForSingleObject(hclintWritten, 3) != WAIT_OBJECT_0)
		{
			loadFromFile();
			SetEvent(hServerWritten);
		}
	}
}

void IPC_test::loadFromMemory()
{

	QBuffer buffer;

	if (!sharedMemory->attach())//将shareMemory与该进程绑定使之可以访问shareMemory里的内容  
	{
		qDebug() << tr("can't attach share memory");
	}

	//写入数据
	sharedMemory->lock();
	buffer.setData(static_cast<const char *>(sharedMemory->constData()), sharedMemory->size());
	buffer.open(QBuffer::ReadWrite);
	sharedMemory->unlock();
	sharedMemory->detach();

	//写入成功则数据增加
	if (buffer.size() != 0)
	{
		byets += ((double)buffer.size()) / 1024;
		//QDateTime currentTime = QDateTime::currentDateTime();
	}
	//增加数据后记录时间
	time_Diff = msTime.elapsed();

	//假如时间已经超过一秒，则显示这一秒钟接收的数据，并将数据量时间量置零
	if (time_Diff > 1000)
	{
		QString stringTem;
		stringTem.sprintf("recive %.1f MB data,use %d ms", byets, time_Diff);
		showText += stringTem;
		showText += "\r\n";
		ui.picLabel->setText(showText);
		byets = 0;
		time_Diff = 0;
		msTime.restart();
	}

}
IPC_test::~IPC_test()
{
	sharedMemory->destroyed();
}
