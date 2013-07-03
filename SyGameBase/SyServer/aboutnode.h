#pragma once
#include "mcenter.h"
#include "singleton.h"
#include "dealcmddelegate.h"
#include "remotefunction.h"
#include "sys.h"
/**
 * ���ӹ���
 */
enum {
	LOGIN_NODE_CONNECT, // ���ӹ���
	LOGIN_USER_CONNECT, // �������
	LOGIN_USER_IN_MANAGER_CONNECT, // ����ڹ������ĵ�����
	MAP_NODE_CONNECT, // �؈D�B��
	NODE_ROOT_CONNECT, // ���ڵ�����
};
/**
 * ��Ϣ���ĳ�
 */
class AboutNode:public MessageCenter,public Singleton<AboutNode>{
public:
    AboutNode(){init();}
    ~AboutNode(){destroy();}
    void init();
};

#define theAboutNode AboutNode::getMe()

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