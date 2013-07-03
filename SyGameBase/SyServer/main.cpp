#include "servernetlib.h"
#include "aboutnode.h"
#include "iocppool.h"
#include "selectpool.h"
#include "epollpool.h"
#include "kqueuepool.h"
#include <sstream>
/**
 * �������߼�
 */
class Server:public ServerNode{
public:
#if __LINUX__
	std::vector<EpollThreadConnectionPoll*> pools;	
#elif __WINDOWS__
	std::vector<IocpPool*> pools;
#else
    std::vector<KqueuePool*> pools;
#endif
	
	void preLoad()
	{
#ifdef __WINDOWS__
		/**
		 * Ԥ����
		 * ��Ҫ�ǳ�ʼ��windows �µ�socket ����
		 */
		IocpPool::LoadSocketLib();
#endif
	}
	void clear()
	{
#ifdef __WINDOWS__
		for (std::vector<IocpPool*>::iterator iter = pools.begin(); iter != pools.end();++iter)
		{
			(*iter)->Stop();
		}
		//pool.Stop();
#endif
	}
	/**
	 * �����������ڵ�
	 */
	bool start(WORD port)
	{
#if __WINDOWS__
		IocpPool *pool = new IocpPool;
#elif __LINUX__
		EpollThreadConnectionPoll * pool = new EpollThreadConnectionPoll;
#else 
        KqueuePool * pool = new KqueuePool;
#endif
		pool->setDealCmdDelegate(&theDeal); // ������Ϣ����Ĵ���
		pool->setSocketCloseDelegate(&theCloseDeal);
		pool->start(port);
		pools.push_back(pool);
		return true;
	}
	/**
	 * ע����Ϊ
	 * \param �ڵ�����
	 * \param ip ��ǰ�ڵ��ip
	 * \param port ��ǰ�ڵ�port
	 * \param nodeType �ڵ�����
	 * \param mapName �ڵ��еĵ�ͼ������
	 **/
	void req(std::string name,std::string ip,WORD port,std::string nodeType,std::string mapName)
	{
		printf("ע��ڵ㵽:%s ��ǰ�ڵ�����:%s ip:%s port:%u\n",name.c_str(),nodeType.c_str(),ip.c_str(),port);
	}
	/**
	 * ���ص�ͼ
	 * \param name ��ͼ����
	 */
	void loadMap(std::string name)
	{
	}
	/**
	 * ж�ع��ܺ���
	 * \param name ��������
	 */
	void downFunction(std::string name)
	{
		theAboutNode.setFuncName(name,NULL,false);
	}
	/**
	 * ���ù����ڽ���ip ��������ʹ��
	 * \param ip �˵��ַ
	 * \param name ��������
	 */
	virtual void setFunctionInUseIp(std::string ip,std::string name)
	{
		theAboutNode.setFuncName(name,NULL,true,ip);
	}
};
/**
 *
 * ������
 * ��ʼ�� nodelib ������
 * //TODO ����30000 �˵�¼ ��ά�������� 
 *    ������loginnodemanager root 
 *    ������login login���������� ��root,loginnodemanager������ ���ܽ����߼���������
 *
 *	1���ͻ��� 1s�з���1������ ȥloginnodemanager ��ѯ��Ч�ڵ�
 *	��ȡlogin �ڵ� �� ��������
 */
 int main(int argc,char*argv[])
 {
	// ���غ�netlib ��
	// �����ͻ�������
	// ��������ڵ���� 
#ifdef __LINUX__
	 daemon(1,1);
#endif
	theAboutNode.init();
	Server server;
	server.preLoad();
	NetLib netlib;
	ConnectionSelectPool pool;
	pool.setDealCmdDelegate(&theDeal);
	pool.setSocketCloseDelegate(&theCloseDeal);
	theClient.setConnectionPool(&pool);
	if (argc == 2)
	{
		netlib.init(argv[1]);
		netlib.execScript(&server,"start"); // ����start �߼�ִ��
	}
	else
	{
		printf("�����������ļ�*.bw\n");
		system("pause");
		return 1;
	}
	pool.start();
	theClient.start();
	printf("\n~~~�����������ɹ�~~~\n");
	sys::theThreadManager.waitAll();
	return 0;
 }
