#include "UIImage.h"
NS_CC_BEGIN

UIImage * UIImage::create(const char *imgName)
{
	UIImage *node = new UIImage();
	if (node)
	{
		if (node->init(imgName))
		{
			node->autorelease();
			return node;
		}
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

bool UIImage::init(const char *imgName)
{
	image = CCSprite::create(imgName);
	if (image)
	{
        this->addChild(image);
		return true;
	}
	return false;
}
 /** 
* ����Ƿ���������
*/
bool UIImage::touchDown(float x,float y)
{
	return false;
}
/**
* ����λ��
*/
bool UIImage::touchMove(float x,float y)
{
	 return false;
}
/**
* ֹͣ�϶�
*/
bool UIImage::touchEnd(float x,float y)
{
	 return false;
}
/**
* ����λ��
*/
void UIImage::setPosition(float x,float y)
{
	if (image)
	{
		image->setPosition(ccp(x,y));
	}
	return ;
}
	
/**
* ���ô�С
*/
void UIImage::setSize(float w,float h)
{
	if (image)
	{
		image->setScaleX(w / image->getContentSize().width);
		image->setScaleY(h / image->getContentSize().height);
	}
	return ;
}
NS_CC_END