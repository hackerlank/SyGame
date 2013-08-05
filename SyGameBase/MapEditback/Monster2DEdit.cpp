#include "Monster2DEdit.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
#include "MapManager.h"
#include "MutiCartoon.h"
#include "MutiImage.h"
#include "MutiMonster.h"
#include "UILib.h"
#include "Cartoon.h"
#include "UIFileList.h"
NS_CC_BEGIN

class DoCreateMonster:public UICallback{
public:
	void callback(UIBase *base)
	{
		
	}
}; // ��ȡ��Ϣ ����һ��Monster

class DoShowDirFrames:public UICallback{
public:
	void callback(UIBase *base)
	{
		
	}
}; // չʾ����һ�������֡

class DoAddFrames:public UICallback{
public:
	void callback(UIBase *base)
	{
		
	}
}; // ����һ������֡
class stAddImage:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel *dirFrameDialog = window->getPanel("monsteredit/add_dir_frames");
			if (dirFrameDialog)
			{
				// �����Ŀ��list
				UISuperBag *list = LIST(dirFrameDialog,"list");
				if (list)
				{
					UIFileItem *fileItem = UIFileItem::create("diritem.xml");
					list->addItem(fileItem);
				}
				list->show();
			}
		}
	}
	stAddImage(UIWindow *window):window(window){}
	UIWindow *window;
}; // ����һ��ͼƬ
class stDirItem:public BaseUIItem<stDirItem>
{
public:
	CartoonXmlAction xmlAction;
}; // ÿ���������Ŀ

class stFramesItem:public BaseUIItem<stFramesItem>{
public:
	CartoonDirAction dirAction;
}; // ÿ������֡

void Monster2DEdit::doInitEvent()
{
	// ���ڲ��߼���ϵͳ��
	UICallbackManager::getMe().addCallback("do_create_monster",new DoCreateMonster()); // �󶨰�ť����Ӧ�¼�
	UICallbackManager::getMe().addCallback("do_show_dir_frames",new DoShowDirFrames()); // �󶨰�ť����Ӧ�¼�
	UICallbackManager::getMe().addCallback("do_add_frames",new DoAddFrames()); // �󶨰�ť����Ӧ�¼�
	UICallbackManager::getMe().addCallback("do_add_image",new stAddImage(window)); // �󶨰�ť����Ӧ�¼�
}

NS_CC_END