#include "AboutRequest.h"
#include "cocos2d.h"
/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION(AboutRequest::getMapInfo)
	PARAM(int,mapWidth);
	PARAM(int ,mapHeight);
	BODY
	{
	}
REMOTE_FUNCTION_END


/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION(AboutRequest::retMapInfo)
	PARAM(int,mapId);
	PARAM(std::string ,mapName);
	BODY
	{
		printf("%s",mapName.c_str());
	}
REMOTE_FUNCTION_END

/**
 * �����¼
 */
IMP_REMOTE_FUNCTION(AboutRequest::reqLogin)
	PARAM(std::string,name);
	PARAM(std::string ,pwd);
	BODY
	{
	}
REMOTE_FUNCTION_END

/**
 * ���س�����Ϣ����
 */
IMP_REMOTE_FUNCTION(AboutRequest::retCanPlay)
	PARAM(std::string,name);
	PARAM(std::vector<stBagInfo>,bags);
	BODY
	{
		// ����UI��չʾ
		// thePinTu->showBags(BagDatas);
	}
REMOTE_FUNCTION_END