#include "AboutRequest.h"
#include "socket.h"
#include "UIManager.h"
using namespace cocos2d;
/**
 * ���󳡾�����Ϣ
 * \param mapWidth ��ͼ�Ŀ�
 * \param mapHeight ��ͼ�ĸ�
 */
IMP_REMOTE_FUNCTION(AboutRequest::retCanPlay)
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
IMP_REMOTE_FUNCTION(AboutRequest::retMapInfo)
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
IMP_REMOTE_FUNCTION(AboutRequest::retLogin)
	PARAM(std::string ,uniqueStr);
	BODY
	{
		// ��¼�ɹ� չʾ�µ�UI ���ߵ����µĳ���
		UIPanel *loginPanel = theUI.getPanel("login");
		if (loginPanel)
		{
			loginPanel->setVisible(false);
		}
		UIPanel *mainPanel = theUI.getPanel("main");
		if (mainPanel)
		{
			mainPanel->setVisible(true);
		}
	}
REMOTE_FUNCTION_END

