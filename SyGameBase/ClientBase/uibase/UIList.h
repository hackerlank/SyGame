#pragma once
#include <vector>
#include "cocos2d.h"
#include "UIItem.h"
#include "UIBase.h"
NS_CC_BEGIN
struct stExecListItem{
	virtual void exec(UIItem *item) = 0;
};
/**
 * List�ռ� �������������
 */
class UIList:public UIBase{
public:
	static UIList* create(const CCRect & viewRect,int eachWidth,int eachHeight);
	/**
	 * ��ֹ������ÿ����Ŀ
	 */
	void hideEach();
	/**
	 * չʾÿ����Ŀ
	 */
	void showEach();
	/**
	 * ��̬������ÿ����Ŀ
	 */
	void hideEachDynamic();
	/**
	 * ����ʹÿ�����嶯̬��չʾ����
	 */
	void showEachDynamic();
	/**
	 * ����λ��
	 */
	virtual void setPosition(float x,float y);
	
	/**
	 * ���ô�С
	 */
	virtual void setSize(float w,float h) {}
	/**
	 * ��β������һ����Ŀ
	 */
	bool pushItem(UIItem *item);
	/**
	 * ɾ��һ����Ŀ
	 */
	bool removeItem(UIItem *item);

	void setStartId(int id){
		startId = id;
	}
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
	
	void setEditable(bool tag){
		editable = tag;
	}
	void execEach(stExecListItem *exec);
	UIItem * getNowItem();
protected:
	/**
	 * ������Ŀ����
	 */
	virtual void doItemDown(UIItem *item);
	int nowDragItemId; // ��ǰ�϶�������ID

	bool editable; // �Ƿ��ڱ༭״̬

	CCPoint nowTouchPoint;
	/**
	 * ���ݼ���
	 */
	std::vector<UIItem*> _items;
	typedef std::vector<UIItem*>::iterator ITEMS_ITER;
	int _viewx; // ��ʼ������
	int _viewy; // ��ʼ������
	int _viewWidth; // ��ͼ��
	int _viewHeight; // ��ͼ��

	int _contentx; // ���ݵĳ�ʼ������
	int _contenty;  // ���ݵĳ�ʼ������
	int _contentWidth; // ���ݿ�
	int _contentHeight; // ���ݸ�

	int _leftItemSpan; // ��߱߿�ľ���
	int _downItemSpan; // ÿ���߿�ľ���
	int _eachItemWidth; // ÿ����Ŀ�Ŀ�
	int _eachItemHeight; // ÿ����Ŀ�ĸ�
	/**
	 * չʾ��ǰ��Ŀ
	 */
	void showItem(UIItem *item);
	/**
	 * ��ȡ���������ɵ���Ŀ
	 */
	int getWidthItemCount();
	int getHeightItemCount();
	CCPoint getRightHidenPosition(int itemId);
	CCPoint getShowPosition(int itemId);
	CCPoint getShowMidPosition(int itemId);
	CCPoint getLeftHidenPosition();
	CCPoint getUpHidenPosition();
	void moveEnded();
	UIList()
	{
		_leftItemSpan = _downItemSpan = _eachItemWidth = _eachItemHeight = 0;
		nowId = startId = 0;
		touchIn = false;
		editable = false;
		uiType = UIBase::UI_LIST;
	}
	int nowId;
	int startId;
	bool touchIn;
};

NS_CC_END