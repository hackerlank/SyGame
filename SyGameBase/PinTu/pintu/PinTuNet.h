#pragma once
#include "socket.h"
#include "remotefunction.h"
#include <string>
#include "sys.h"
/**
 * ���ڵ�½����
 * �ͻ�������LoginCenter ������Ч�ĵ�½�ڵ�,
 * LoginCenter ����Login ����פ�����
 * ���ݴ���Ч��,����
 */
class HeXieNet:public RemoteFunction,public Singleton<HeXieNet>,public CallDelegate{
public:
	HeXieNet():CallDelegate("aboutnet")
	{
	
	}
	/**
	 * ���󳡾�����Ϣ
	 * \param mapWidth ��ͼ�Ŀ�
	 * \param mapHeight ��ͼ�ĸ�
	 */
	 DEC_REMOTE_FUNCTION_2(getMapInfo,
		int mapWidth,int mapHeight)
	{
		PRE_CALL_2(getMapInfo,mapWidth,mapHeight);
	}
};

#define theNet HeXieNet::getMe()