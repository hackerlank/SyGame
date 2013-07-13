#pragma once
#include <vector>
#include "cocos2d.h"
#include "xmlScript.h"

NS_CC_BEGIN
class UIPanel;
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

class XmlUIItem:public UIItem{
public:
	static XmlUIItem* create(script::tixmlCodeNode *node);
	bool initWithNode(script::tixmlCodeNode *node);
	XmlUIItem()
	{
		panel = NULL;
	}
	virtual bool doTouch(int touchType,const CCPoint &touchPoint);
	UIPanel * getPanel();
	virtual void setSize(float width,float height);
private:
	UIPanel *panel;
};
template<typename CHILD>
class BaseUIItem:public XmlUIItem,public script::tixmlCode{
public:
	static CHILD * create(const std::string &name)
	{
		CHILD *node = new CHILD();
		if (node)
		{
			node->init(name);
			node->autorelease();
			return node;
		}
		CC_SAFE_DELETE(node);
		return NULL;
	}

	bool init(const std::string &name)
	{
		std::string startui = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(name);
		unsigned long nSize = 0;
		unsigned char * buffer = CCFileUtils::sharedFileUtils()->getFileData(startui.c_str(),"rb",&nSize);
		if (!nSize)return false;
		if (script::tixmlCode::initFromString((char*)buffer))
		{
			doInitEvent();
			return true;
		}
		return false;
	}
	/**
	 * �������ļ��м�������
	 * \param node ���ø��ڵ�
	 */
	void takeNode(script::tixmlCodeNode *node)
	{
		if (node && node->equal("Config"))
		{
			XmlUIItem::initWithNode(&node);
		}
		vTakeNode(node);
	}
	virtual void vTakeNode(script::tixmlCodeNode *node){}
protected:
	virtual void doInitEvent(){}
};
NS_CC_END
