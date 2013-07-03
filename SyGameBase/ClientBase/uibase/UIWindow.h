#pragma once
/**
 * ȫ�ֱ���
 * ϵͳ�г���dialog dialog����Ƕ��
 */
#include "cocos2d.h"
#include "UIPanel.h"
NS_CC_BEGIN
class UIWindow:public CCNode{
public:
	static UIWindow& getMe();

	static UIWindow * create();
	/**
	 * ����һ������
	 */
	void addPanel(UIPanel *panel);

	/** 
	 * ����Ƿ���������
	 */
	bool touchDown(float x,float y);
	/**
	 * ����λ��
	 */
	bool touchMove(float x,float y);
	/**
	 * ֹͣ�϶�
	 */
	bool touchEnd(float x,float y);

	void attachParent(CCNode *node);

	UIPanel * getNowTouchPanel(){return _nowTouchPanel;}
	/**
	 * ɾ����ǰ�Panel
	 */
	void removeNowPanel();
	/**
	 * �������ͼƬ
	 */
	void setCursor(CCSprite *sprite);
	/**
	 * ����ui
	 */
	void addUI(UIBase *base);

	void setNowTouchPanel(UIPanel * pan)
	{
		if (_nowTouchPanel) _nowTouchPanel->setZOrder(0);
		_nowTouchPanel = pan;
		if (pan)
			pan->setZOrder(3);
	}
	static bool isClose()
	{
		return !window;
	}
	CCPoint getCursorPoint();
	static void release();

	void setEndFunction(const FunctionInfo &function);
	/**
	 * չʾ��ǰ������Ϣ
	 */
	void showErr(const std::string& info);
private:
	CCLabelTTF * _errLbl;
	void insertBase(UIBase *base);
	typedef std::list<UIPanel*> PANELS;
	PANELS _panels;
	typedef PANELS::iterator PANELS_ITER;
	typedef std::list<UIBase*> BASES;
	BASES _bases;
	typedef BASES::iterator BASES_ITER;
	UIPanel *_nowTouchPanel;
	CCSprite *_cursor; // ����α�
	UIBase *_nowTouchUI;
	CCPoint nowCursorPoint;
	bool endFlag;
	UIWindow()
	{
		_errLbl = NULL;
		endFlag = false;
		_nowTouchUI = NULL;
		_cursor = NULL;
		_nowTouchPanel = NULL;
	}
	static UIWindow * window;
};
NS_CC_END
