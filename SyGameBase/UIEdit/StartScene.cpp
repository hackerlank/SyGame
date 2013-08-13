#include "UIWindow.h"
#include "StartScene.h"
#include "StartDialog.h"
#include <pthread.h>
#include "PngPack.h"
USING_NS_CC;

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
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	window = UIWindow::create();
	if (!window) return false;
	
	this->addChild(window);
	if ( !CCLayer::init() )
    {
        return false;
    }
	
    this->setTouchEnabled(true);
   
	//startDialog = StartDialog::create(window,"startui.xml");
	
	/**
	 * ��������
	 */
	MainDialog *mainDialog = MainDialog::create(window,"mainui.xml");
	
	/**
	 * �������
	 */
	SavePanelDialog *saveDialog = SavePanelDialog::create(window,"savedialog.xml");
	saveDialog->setVisible(false);
	
	/**
	 * ���Խ���
	 */
	PropDialog *propDialog = PropDialog::create(window,"prop.xml");
	

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
	if (window->touchDown(pos.x,pos.y)) return;
	
}
void StartScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
    CCPoint pos = touch->getLocation();
	/**
	 * ������
	 */
	if (window->touchMove(pos.x,pos.y)) return;
}
void StartScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
    CCPoint pos = touch->getLocation();
	/**
	 * ������
	 */
	window->touchEnd(pos.x,pos.y);
}
void StartScene::step(float dt)
{

}