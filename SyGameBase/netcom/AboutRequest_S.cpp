#include "AboutRequest.h"
#include "socket.h"
/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION2(AboutRequest,retCanPlay)
	PARAM(std::vector<stBagInfo>,bags);
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
		// ��¼�ɹ� չʾ�µ�UI ���ߵ����µĳ���
		
	}
REMOTE_FUNCTION_END

/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION2(AboutRequest,getMapInfo)
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
IMP_REMOTE_FUNCTION2(AboutRequest,reqLogin)
	PARAM(std::string,name);
	PARAM(std::string,pwd);
	BODY
	{
		printf("reqLogin %s %s\n",name.c_str(),pwd.c_str());
		retLogin(socket,"tickyou");
	}
REMOTE_FUNCTION_END