#include "PinTuAboutNet.h"
#include "sstream"
#include "netlib.h"
#include "AboutRequest.h"

/**
 * ������Ϣ
 **/
class DealCmd:public DealCmdDelegate,public Singleton<DealCmd>,public LocalCallDelegate{
public:
	/**
	 * ������Ϣ
	 * \param socket ����
	 * \param cmd ��Ϣ
	 * \param size ��Ϣ��С
	 */
	void doObjectCmd(Socket *socket,void *cmd,int size);
	/**
	 * ��������Ϣ
	 * \param socket ���Ӿ��
	 * \param object ��Ϣ����
	 */
	void doLocalCmd(Socket *socekt,void *cmd,int size);

	/**
	 * �����������
	 * \param socket ���Ӿ��
	 * \param cmd ��Ϣ
	 * \param size ��Ϣ��С
	 */
	void doPackCmd(Socket *socekt,void *cmd,int size);
};
/**
 * �������ӵĹر�
 */
class DealClose:public SocketCloseDelegate,public Singleton<DealClose>{
public:
	/**
	 * �������ӵ��ͷ�
	 * \param socket ���Ӷ���
	 */
	void handleClose(Socket *socket);
	/**
	 * �������ӵ�����
	 * \param socket ���Ӷ���
	 */
	void handleAdd(Socket *socket);
	/**
	 *  �����i
	 **/
	sys::Mutex _mutex;
};
#define theDeal DealCmd::getMe()
#define theCloseDeal DealClose::getMe()

/**
 * ������Ϣ
 * \param socket ����
 * \param cmd ��Ϣ
 * \param size ��Ϣ��С
 */
void DealCmd::doObjectCmd(Socket *socket,void *cmd,int size)
{
	ENTER_FUNCTION
	theNet.handle(&theRequest,cmd,size,socket);
};


/**
 * �������ӵ����Ӹ�
 * \param socket ���Ӷ���
 */
void DealClose::handleAdd(Socket *socket)
{
	
}
/**
 * �������ӵ��ͷ�
 * \param socket ���Ӷ���
 */
void DealClose::handleClose(Socket *socket)
{
	ENTER_FUNCTION
	_mutex.lock();
	ClientNode * client = theClient.addRemoteClient("request");
	if (client)
	{
		if (socket == client->getSocket())
		{
			printf("xxx");
		}
	}
	_mutex.unlock();
}

/**
 * ��������Ϣ
 * \param socket ���Ӿ��
 * \param object ��Ϣ����
 */
void DealCmd::doLocalCmd(Socket *socket,void *cmd,int size)
{
	// ������
};
/**
 * �����������
 * ���������Կͻ�����Ϣ�Ĵ���
 * \param socket ���Ӿ��
 * \param cmd ��Ϣ
 * \param size ��Ϣ��С
 */
void DealCmd::doPackCmd(Socket *socket,void *cmd,int size)
{
	// ������
}
AboutNet theNet ;
/**
 * �ڵ���Ϣ���ĵĳ�ʼ��
 **/
void AboutNet::init()
{
	static bool init = false;
	if (!init) // ȷ����ʼ��ֻ��һ��
	{
		// ��½����
		theRequest.init(this);
		init = true;
		ConnectionSelectPool *pool = new ConnectionSelectPool();
		pool->setDealCmdDelegate(&theDeal);
		pool->setSocketCloseDelegate(&theCloseDeal);
		theClient.setConnectionPool(pool);
		pool->start();
		theClient.start();
		WSADATA wsaData;
		int nResult;
		nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		// ����(һ�㶼�����ܳ���)
		if (NO_ERROR != nResult)
		{
	//		this->_ShowMessage(_T("��ʼ��WinSock 2.2ʧ�ܣ�\n"));
			return; 
		}	

	}
}
Socket * AboutNet::getSocket()
{
	if (!client)
	{
		connect();
	}
	if (!client) return NULL;
	std::string ip="localhost";
	WORD port = 4008;
	if (!client->checkValid())
	{
		client->getSocket()->setInvalid();
		client->reconnect(ip.c_str(),port);
	}
	return client->getSocket();
}

bool AboutNet::connect()
{
	client = theClient.addRemoteClient("request");
	std::string ip="localhost";
	WORD port = 4008;
	if (client)
	{
		if (!client->connect(ip.c_str(),port))
		{
			client->getSocket()->setInvalid();
			return false;
		}
		else
		{
			//client->reconnect(ip.c_str(),port);
			theRequest.getMapInfo(client->getSocket(),100,100); // ��������map����Ϣ
			//client->getSocket()->setInvalid();
		}
	}
	return true;
}