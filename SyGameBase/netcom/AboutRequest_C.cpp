#include "AboutRequest.h"
#include "socket.h"

/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
/*
IMP_REMOTE_FUNCTION2(AboutRequest,retCanPlay)
	PARAM(std::vector<stBagInfo>,bags);
	BODY
	{
		printf("���������ص���");
	}
REMOTE_FUNCTION_END
*/
/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION2(AboutRequest,retMapInfo)
	PARAM(int,mapId);
	PARAM(std::string ,mapName);
	BODY
	{
		printf("���������ص���");
	}
REMOTE_FUNCTION_END

/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
 IMP_REMOTE_FUNCTION2(AboutRequest,retLogin)
	PARAM(std::string ,uniqueStr);
	BODY
	{
		
	}
REMOTE_FUNCTION_END

