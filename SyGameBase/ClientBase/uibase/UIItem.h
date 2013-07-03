#pragma once
#include <vector>
#include "cocos2d.h"
NS_CC_BEGIN
/**
 * ʵ�����ɹ̶���С��sprite �ҿռ���Դ���չʾ�Ŀռ� �����϶����ݣ����ҷ���
 */
class UIBag;
class UIItem:public CCNode{
public:
	/**
	 * �ڰ����е�λ��
	 */
	int bagId;
	virtual void setSize(float width,float height);
	void hide();
	void show(const CCRect& viewRect);
	void setPosition(float x,float y);
	void show();
	virtual void visit();
	CREATE_FUNC(UIItem);
	virtual bool init(){return true;}
	/**
	 * �����Ƿ���Item ��
	 */
	bool checkIn(float x,float y);
	virtual UIItem *clone(); // �����Լ�һ��
	/**
	 * ����������ƶ�����λ�÷�
	 */
	virtual bool canMove(int bagId){return true;}
	virtual void setOpacity(float o){}
	/**
	 * ������
	 * \parma touchType �������
	 * \param touchPoint �����
	 */
	virtual bool doTouch(int touchType,const CCPoint &touchPoint){return false;}
protected:
	friend class UIBag;
	UIBag * _bag;
	CCRect _viewRect;
	CCRect _oldRect;
	bool downTag; // ��ʶ�Ƿ��°�ť
	bool showInView;
	UIItem()
	{
		showInView = false;
		downTag = false;
		bagId = 0;
		_bag = NULL;
	}
};

NS_CC_END
