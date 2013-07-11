#pragma once
#include "remotefunction.h"
#include <string>
#include "AboutData.h"
class AboutRequest:public RemoteFunction,public Singleton<AboutRequest>,public CallDelegate{
public:
	MAKE_REMOTE_CLASS(AboutRequest)
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
	/**
	 * ���س�����Ϣ
	 */
	DEC_REMOTE_FUNCTION_2(retMapInfo,int mapId,const std::string& mapName)
	{
		PRE_CALL_2(retMapInfo,mapId,mapName);
	}

	/**
	 * �����¼��ϵͳ
	 */
	DEC_REMOTE_FUNCTION_2(reqLogin,const std::string &name,const std::string &pwd)
	{
		PRE_CALL_2(reqLogin,name,pwd);
	}
	/**
	 * ��¼�ɹ��󷵻�
	 */
	DEC_REMOTE_FUNCTION_1(retLogin,const std::string &uniqueStr)
	{
		PRE_CALL_1(retLogin,uniqueStr);
	}
	/**
	 * ���ص�ǰ���������Ϣ
	 */
	DEC_REMOTE_FUNCTION_1(retCanPlay,std::vector<stBagInfo>& bags)
	{
		PRE_CALL_1(retCanPlay,bags);
	}
};


#define theRequest AboutRequest::getMe()