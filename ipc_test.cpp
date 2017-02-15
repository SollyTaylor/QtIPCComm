#include "ipc_test.h"

const char *KEY_SHARED_MEMORY = "Shared";

IPC_test::IPC_test(int mode,QWidget *parent)
	: QMainWindow(parent),
	sharedMemory(new QSharedMemory(KEY_SHARED_MEMORY, this))
{
	ui.setupUi(this);

	//ui.picLabel->adjustSize();
	//ui.picLabel->setGeometry(QRect(100, 0, 600, 60 * 4));  //四倍行距
	ui.picLabel->setWordWrap(true);
	ui.picLabel->setAlignment(Qt::AlignTop);
	ui.picLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观

	byets = 0;
	sharedMemory->setKey("sharedMemory");
	//设置程序格式
	this->ipcMode = mode;
	//客户端程序启动时开始计时
	timeSinStart = 0;
	time_Diff = 0;

	serverFirst = true;
	clientRecieve = false;

	//设置客户端检测的定时器
	timerClient = new QTimer(this);
	connect(timerClient, SIGNAL(timeout()), this, SLOT(timerUpdateClient()));

	//设置服务端检测的定时器
	timerServer = new QTimer(this);
	connect(timerServer, SIGNAL(timeout()), this, SLOT(timerUpdateServer()));

	pubTimer = new QTimer(this);
	connect(pubTimer, SIGNAL(timeout()), this, SLOT(serverPub()));

	subTimer=new QTimer(this);
	connect(subTimer, SIGNAL(timeout()), this, SLOT(clientSub()));

	pubClientNum = 0;//初始订阅客户端数量为0
	pubClientRecieved = 0;//接收到数据的客户端数目也为0


	//设置运行与否

	//选择图像（作为最基本的数据单元）（更改为自动去test文件夹下找数据图片）
	//connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
	//加载图片到共享内存
	//connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	//从内存加载到显示
	//connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadFromMemory()));

	LPCSTR ClintAsk = (LPCSTR)"ClintAsk";

	LPCSTR ClientWriteIng = (LPCSTR)"ClientWriteIng";
	LPCSTR ServerWriteIng = (LPCSTR)"ServerWriteIng";

	LPCSTR ClientWriteOver = (LPCSTR)"ClientWriteOver";
	LPCSTR ServerWriteOver = (LPCSTR)"ServerWriteOver";

	LPCSTR ClientSub = (LPCSTR)"ClientSub";

	//LPCSTR ServerWriteOver = (LPCSTR)"ServerWriteOver";
	hclintAsk = CreateEventA(NULL, TRUE, FALSE, ClintAsk);
	hClintWriting = CreateEventA(NULL, TRUE, FALSE, ClientWriteIng);
	hServerWriting = CreateEventA(NULL, TRUE, FALSE, ServerWriteIng);
	hServerWritten = CreateEventA(NULL, TRUE, FALSE, ServerWriteOver);
	hclintWritten = CreateEventA(NULL, TRUE, FALSE, ClientWriteOver);
	hclientSub = CreateEventA(NULL, TRUE, FALSE, ClientSub);

	ResetEvent(hclintAsk);
	ResetEvent(hClintWriting);
	ResetEvent(hServerWriting);
	ResetEvent(hServerWritten);
	ResetEvent(hclintWritten);
	ResetEvent(hclientSub);


	initialize();
}

void IPC_test::initialize()
{

	QDateTime startTime = QDateTime::currentDateTime();
	showText = "start time:" + startTime.toString("yyyy-MM-dd hh:mm:ss ddd") + "\r\n";
	ui.picLabel->setText(showText);
	if (this->ipcMode == IPC_TEST || this->ipcMode == IPC_CLINT_REG || this->ipcMode == IPC_CLINT_SUB)
	{
		timerClient->start(3);
		showText += "this is client\r\n";
		ui.picLabel->setText(showText);
	}
	if (this->ipcMode == IPC_TEST || this->ipcMode == IPC_SERVER_REP || this->ipcMode == IPC_SERVER_PUB)
	{
		timerServer->start(3);
		showText += "this is server\r\n";
		ui.picLabel->setText(showText);
	}
	if (this->ipcMode == IPC_SERVER_PUB)
	{
		pubTimer->start(1);
	}
	if (this->ipcMode == IPC_CLINT_SUB)
	{
		subTimer->start(1);
	}
	//开始计时
	msTime.start();
}


void IPC_test::timerUpdateClient()
{
	if (this->ipcMode == IPC_TEST)
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
	else if (this->ipcMode == IPC_CLINT_REG)
	{
		if (WaitForSingleObject(hclintAsk, 1) != WAIT_OBJECT_0)//假如没有客户端申请数据则申请数据
		{
			SetEvent(hclintAsk);
			if (WaitForSingleObject(hServerWritten, 3000) == WAIT_OBJECT_0)//等待客户端3s写入数据
			{
				loadFromMemory();
				ResetEvent(hServerWritten);
				ResetEvent(hclintAsk);
			}
		}
	}
	else if (this->ipcMode == IPC_CLINT_SUB)
	{

	}
}
void IPC_test::clientSub()
{
	if (WaitForSingleObject(hServerWritten, 3) == WAIT_OBJECT_0)//检测服务端是否写完数据
	{
		clientRecieve = false;
	}
	if (clientRecieve == false)
	{
		if (WaitForSingleObject(hClintWriting, 3) != WAIT_OBJECT_0)//检测服务端是否已经统计完上个客户端的接收信息
		{
			//ResetEvent(hclintWritten);
			SetEvent(hClintWriting);
			loadFromMemory();
			ResetEvent(hClintWriting);
			SetEvent(hclintWritten);
			clientRecieve = true;
		}
	}
}
void IPC_test::timerUpdateServer()
{
	if (this->ipcMode == IPC_TEST)
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
	else if (this->ipcMode == IPC_SERVER_REP)
	{
		if (WaitForSingleObject(hclintAsk, 3) == WAIT_OBJECT_0)//等待到了客户端的数据请求
		{
			loadFromFile();
			SetEvent(hServerWritten);
		}
	}
	else if (this->ipcMode == IPC_SERVER_PUB)
	{
		if (WaitForSingleObject(hclientSub, 3) != WAIT_OBJECT_0)//这部分用于客户端数量更新
		{
			pubClientNum++;
			ResetEvent(hclientSub);
		}
	}
}

void IPC_test::serverPub()
{
	if (pubClientRecieved>=5||serverFirst)//所有客户端完成数据接收或者是客户端第一次发送数据
	{
		ResetEvent(hServerWritten);
		loadFromFile();
		SetEvent(hServerWritten);
		serverFirst = false;
		//showText += "send message success \r\n";
		//ui.picLabel->setText(showText);
		pubClientRecieved = 0;
	}
	else if (WaitForSingleObject(hclintWritten, 3) == WAIT_OBJECT_0)
	{
		ResetEvent(hclintWritten);
		pubClientRecieved++;
	}
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
