#pragma once
#include "cocos2d.h"
#include "UIBase.h"
NS_CC_BEGIN

class UIImage :public UIBase{
public:
	static UIImage * create(const char *imgName);

	bool init(const char *imgName);
	 /** 
	 * ����Ƿ���������
	 */
	virtual bool touchDown(float x,float y);
	/**
	 * ����λ��
	 */
	virtual bool touchMove(float x,float y);
	/**
	 * ֹͣ�϶�
	 */
	virtual bool touchEnd(float x,float y);
	/**
	 * ����λ��
	 */
	virtual void setPosition(float x,float y);
	
	/**
	 * ���ô�С
	 */
	virtual void setSize(float w,float h);
	void setEditable(bool tag){
		_editable = tag;
	}
private:
	bool _editable;
	CCSprite *image;
	UIImage()
	{
		_editable = false;
		image = NULL;
	}
};

NS_CC_END