#include "ipc_test.h"

const char *KEY_SHARED_MEMORY = "Shared";

IPC_test::IPC_test(int mode,QWidget *parent)
	: QMainWindow(parent),
	sharedMemory(new QSharedMemory(KEY_SHARED_MEMORY, this))
{
	ui.setupUi(this);

	//ui.picLabel->adjustSize();
	//ui.picLabel->setGeometry(QRect(100, 0, 600, 60 * 4));  //�ı��о�
	ui.picLabel->setWordWrap(true);
	ui.picLabel->setAlignment(Qt::AlignTop);
	ui.picLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); //�������

	byets = 0;
	sharedMemory->setKey("sharedMemory");
	//���ó����ʽ
	this->ipcMode = mode;
	//�ͻ��˳�������ʱ��ʼ��ʱ
	timeSinStart = 0;
	time_Diff = 0;

	serverFirst = true;
	clientRecieve = false;

	//���ÿͻ��˼��Ķ�ʱ��
	timerClient = new QTimer(this);
	connect(timerClient, SIGNAL(timeout()), this, SLOT(timerUpdateClient()));

	//���÷���˼��Ķ�ʱ��
	timerServer = new QTimer(this);
	connect(timerServer, SIGNAL(timeout()), this, SLOT(timerUpdateServer()));

	pubTimer = new QTimer(this);
	connect(pubTimer, SIGNAL(timeout()), this, SLOT(serverPub()));

	subTimer=new QTimer(this);
	connect(subTimer, SIGNAL(timeout()), this, SLOT(clientSub()));

	pubClientNum = 0;//��ʼ���Ŀͻ�������Ϊ0
	pubClientRecieved = 0;//���յ����ݵĿͻ�����ĿҲΪ0


	//�����������

	//ѡ��ͼ����Ϊ����������ݵ�Ԫ��������Ϊ�Զ�ȥtest�ļ�����������ͼƬ��
	//connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
	//����ͼƬ�������ڴ�
	//connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	//���ڴ���ص���ʾ
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
	//��ʼ��ʱ
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
		if (WaitForSingleObject(hclintAsk, 1) != WAIT_OBJECT_0)//����û�пͻ���������������������
		{
			SetEvent(hclintAsk);
			if (WaitForSingleObject(hServerWritten, 3000) == WAIT_OBJECT_0)//�ȴ��ͻ���3sд������
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
	if (WaitForSingleObject(hServerWritten, 3) == WAIT_OBJECT_0)//��������Ƿ�д������
	{
		clientRecieve = false;
	}
	if (clientRecieve == false)
	{
		if (WaitForSingleObject(hClintWriting, 3) != WAIT_OBJECT_0)//��������Ƿ��Ѿ�ͳ�����ϸ��ͻ��˵Ľ�����Ϣ
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
		if (WaitForSingleObject(hclintAsk, 3) == WAIT_OBJECT_0)//�ȴ����˿ͻ��˵���������
		{
			loadFromFile();
			SetEvent(hServerWritten);
		}
	}
	else if (this->ipcMode == IPC_SERVER_PUB)
	{
		if (WaitForSingleObject(hclientSub, 3) != WAIT_OBJECT_0)//�ⲿ�����ڿͻ�����������
		{
			pubClientNum++;
			ResetEvent(hclientSub);
		}
	}
}

void IPC_test::serverPub()
{
	if (pubClientRecieved>=5||serverFirst)//���пͻ���������ݽ��ջ����ǿͻ��˵�һ�η�������
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

	if (!sharedMemory->attach())//��shareMemory��ý��̰�ʹ֮���Է���shareMemory�������  
	{
		qDebug() << tr("can't attach share memory");
	}

	//д������
	sharedMemory->lock();
	buffer.setData(static_cast<const char *>(sharedMemory->constData()), sharedMemory->size());
	buffer.open(QBuffer::ReadWrite);
	sharedMemory->unlock();
	sharedMemory->detach();

	//д��ɹ�����������
	if (buffer.size() != 0)
	{
		byets += ((double)buffer.size()) / 1024;
		//QDateTime currentTime = QDateTime::currentDateTime();
	}
	//�������ݺ��¼ʱ��
	time_Diff = msTime.elapsed();

	//����ʱ���Ѿ�����һ�룬����ʾ��һ���ӽ��յ����ݣ�����������ʱ��������
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
