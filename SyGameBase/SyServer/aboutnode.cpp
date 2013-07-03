#include "aboutnode.h"
#include "sstream"
#include "AboutRequest.h"

/**
 * ������Ϣ
 * \param socket ����
 * \param cmd ��Ϣ
 * \param size ��Ϣ��С
 */
void DealCmd::doObjectCmd(Socket *socket,void *cmd,int size)
{
	ENTER_FUNCTION
	if (theAboutNode.handle(&theRequest,cmd,size,socket))
	{}
};


/**
 * �������ӵ����Ӹ�
 * \param socket ���Ӷ���
 */
void DealClose::handleAdd(Socket *socket)
{
//	if (socket)
//		socket->setInvalidDelay(3000); // 3000 ms���Զ��Ͽ�
}
/**
 * �������ӵ��ͷ�
 * \param socket ���Ӷ���
 */
void DealClose::handleClose(Socket *socket)
{
	ENTER_FUNCTION
	_mutex.lock();
	_mutex.unlock();
}

/**
 * ��������Ϣ
 * \param socket ���Ӿ��
 * \param object ��Ϣ����
 */
void DealCmd::doLocalCmd(Socket *socket,void *cmd,int size)
{
	ENTER_FUNCTION
	if (theAboutNode.handle(&theRequest,cmd,size,socket,LOCAL_CALL_FUNCTION))
	{}
	
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
	ENTER_FUNCTION
}
/**
 * �ڵ���Ϣ���ĵĳ�ʼ��
 **/
void AboutNode::init()
{
	static bool init = false;
	if (!init) // ȷ����ʼ��ֻ��һ��
	{
		theRequest.init(this);
		init = true;
	}
}
