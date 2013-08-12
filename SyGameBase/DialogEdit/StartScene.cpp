#include "UIWindow.h"
#include "StartScene.h"

USING_NS_CC;
std::string gloabFileName;
/**
 * �����߼�
 */
class stDoSaveDialog:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel * target = window->showPanel(gloabFileName.c_str());
			if (target)
			{
				target->makeXmlFile(gloabFileName.c_str());
			}
		}
	}
	stDoSaveDialog(UIWindow *window):window(window){}
	UIWindow *window;
};
/**
 * ���߼�
 */
class stDoOpenDialog:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel *panel = base->getPanel();
			if (panel)
			{
				// ��ȡ�����ֶε�ֵ
				std::string fileName = panel->getUILablevalue("filename");
				UIPanel * target = window->showPanel(fileName.c_str());
				if (target)
				{
					target->setEditable(true);
				}
				gloabFileName = fileName;
				panel->hide();
			}
		}
	}
	stDoOpenDialog(UIWindow *window):window(window){}
	UIWindow *window;
};
class LogicCallback:public UIEventCallback{
public:
	void init(UIWindow *window)
	{
		// �����߼��ص�
		UICallbackManager::getMe().addCallback("do_save_dialog",new stDoSaveDialog(window));

		UICallbackManager::getMe().addCallback("do_add_dialog",new stDoOpenDialog(window));
	}
};

CCScene* StartScene::scene()
{
    CCScene *scene = CCScene::create();
    StartScene *layer = StartScene::create();
    scene->addChild(layer);
    return scene;
}

/**
 * һ���򵥵���Ϸ
 * ����ֱ���� �ж�ʱ���� �������Ǹ�BAG
 */

bool StartScene::init()
{
	theUILib.initWithFile("uilib.xml");

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	LogicCallback initLogic;
	mainUI = UIMain::create(this,&initLogic);
	if ( !CCLayer::init() )
    {
        return false;  
    }
	
    this->setTouchEnabled(true);

	this->schedule(schedule_selector(StartScene::step), 0.75f);   
	
	return true;
	
}

void StartScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
    CCPoint pos = touch->getLocation();
	
	/**
	 * ������
	 */
	if (mainUI->touchDown(pos.x,pos.y)) return;
	
}
void StartScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
    CCPoint pos = touch->getLocation();
	/**
	 * ������
	 */
	if (mainUI->touchMove(pos.x,pos.y)) return;
}
void StartScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
    CCPoint pos = touch->getLocation();
	/**
	 * ������
	 */
	mainUI->touchEnd(pos.x,pos.y);
}
void StartScene::step(float dt)
{

}