#pragma once 
#include "cocos2d.h"
#include "UISuperBag.h"
#include "xmlScript.h"
#include "AboutData.h"
/**
 * ��г����Ŀ
 */
NS_CC_BEGIN
/**
 * �����е���Ŀ
 */
class PinTuItem:public UIItem{
public:
	/**
	 * ������Ŀ չʾ���ǵ���ͼ�� ����������������ͬ�Ĵ��� �������С 6 * 6 ÿ����Ŀ 64 * 64
	 */
	static PinTuItem*create(CCTexture2D * texture,const CCRect &rect);
	bool init(CCTexture2D * texture,const CCRect &rect);
	
	bool canMove(int bagId);
	virtual void setSize(float w,float h);
	CCSprite *content;
	PinTuItem()
	{
		content = NULL;
		positionId = 0;
	}
	int positionId; // λ����Ϣ
};

/**
 * ���� ����һ��BAG ��������ͼƬ ������
 **/
class PinTuBag:public UISuperBag{
public:
	static PinTuBag* create(const CCSize &size,const char *pngName);
	bool init(const CCSize &size,const char *pngName);
	int nowItemId;
	virtual bool setItem(UIItem *item,int id);
	
	/**
	 * ��ȡ��ǰ���ӵĿ�
	 */
	float getPixelWidth();
	/**
	 * ��ȡ��ǰ���ӵĸ�
	 **/
	float getPixelHeight();
	
	/**
	 * ͨ��bagid��ȡzpos
	 */
	//zPos getZPosByBagId(int bagId);
	CCPoint getPixelPosition(int x,int y);
	CCSize eachSize;
	void setPosition(const CCPoint &point);
public:
	/**
	 * ��ȡ��ǰid ������λ��
	 */
	virtual CCPoint getPixelPosition(int id);

	virtual int getNowTouchBagIdByCursorPosition(const CCPoint& pos);
	/**
	 * ��������Ƿ���������
	 */
	virtual bool checkIn(int x,int y);

	float _width; // ��
	float _height; // ��
	float _eachUpSpan; // ÿ���� span
	float _eachLeftSpan; // ÿ����span
	PinTuBag()
	{
		_width = _height = 0;
		_eachUpSpan = _eachLeftSpan = 0;
		nowItemId = -1;
	}
	float getViewWidth();
	float getViewHeight();
};
NS_CC_END