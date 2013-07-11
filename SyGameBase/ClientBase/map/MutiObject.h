#pragma once
#include "cocos2d.h"
#include "xmlScript.h"
NS_CC_BEGIN

class MutiObject :public CCSprite{
public:
	/**
	 * �������ļ��ж�ȡ��Ϣ
	 */
	virtual void readNode(script::tixmlCodeNode *node);
	/**
	 * ����Ϣд��ڵ㵱��
	 */
	virtual TiXmlElement * writeNode(TiXmlElement *parent,const std::string &name);

	bool checkIn(const CCPoint &point);
};

NS_CC_END