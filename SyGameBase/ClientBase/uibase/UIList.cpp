#include "UIList.h"
NS_CC_BEGIN
UIList* UIList::create(const CCRect & viewRect,int eachWidth,int eachHeight)
{
	UIList *bag = new UIList();
	bag->_viewx= viewRect.origin.x;
	bag->_viewy = viewRect.origin.y;
	bag->_viewWidth = viewRect.size.width;
	bag->_viewHeight = viewRect.size.height;
	bag->_eachItemWidth = eachWidth;
	bag->_eachItemHeight = eachHeight;
    if (bag)
    {
        bag->autorelease();
        return bag;
    }
    CC_SAFE_DELETE(bag);
    return NULL;
}
/**
* ��ֹ������ÿ����Ŀ
*/
void UIList::hideEach()
{
	for (ITEMS_ITER iter = _items.begin(); iter != _items.end(); ++iter)
	{
		UIItem * item = *iter;
		if (item)
		{
			CCPoint rightHiddenPos = getRightHidenPosition(item->bagId - startId);
			item->setPosition(rightHiddenPos.x,rightHiddenPos.y);
			item->setVisible(false);
		}
	}
}
/**
* ��̬������ÿ����Ŀ
*/
void UIList::hideEachDynamic()
{
	
}
/**
* ����ʹÿ�����嶯̬��չʾ����
*/
void UIList::showEachDynamic()
{
	hideEach();
	nowId = startId;
	if (nowId < _items.size() && nowId - startId < getHeightItemCount())
	{
		UIItem * item = _items.at(nowId);
		
		if (item)
		{
			item->show();
			CCPoint pos = getShowMidPosition(item->bagId - startId);
			CCMoveTo *action = CCMoveTo::create(0.1f,pos);
			item->runAction(CCSequence::create(action,
					CCCallFunc::create(this, callfunc_selector(UIList::moveEnded)),NULL));
		}
	}
}
/**
 * չʾÿ����Ŀ
 */
void UIList::showEach()
{
	hideEach();
	for ( int nowId = startId; nowId - startId < getHeightItemCount() && nowId < _items.size(); nowId++)
	{
		UIItem * item = _items.at(nowId);
		if (item)
		{
			item->show();
			CCPoint pos = getShowPosition(item->bagId - startId);
			item->setPosition(pos.x,pos.y);
		}
	}
}
/**
* ��β������һ����Ŀ
*/
bool UIList::pushItem(UIItem *item)
{
	if (!item) return false;
	item->bagId = _items.size();
	item->setSize(_eachItemWidth,_eachItemHeight);
	_items.push_back(item);
	item->hide();
	item->show(CCRectMake(_viewx,_viewy,_viewWidth,_viewHeight));

	this->addChild(item);

	return true;
}
/**
* ɾ��һ����Ŀ
*/
bool UIList::removeItem(UIItem *item)
{
	return false;
}

/**
* չʾ��ǰ��Ŀ
*/
void UIList::showItem(UIItem *item)
{

}
#define LIST_H _viewy + _viewHeight - (itemId + 1)* (_eachItemHeight + _downItemSpan)
/**
* ��ȡ���������ɵ���Ŀ
*/
int UIList::getWidthItemCount()
{
	return _contentWidth / ( _leftItemSpan + _eachItemWidth );
}
int UIList::getHeightItemCount()
{
	return _viewHeight / (_eachItemHeight + _downItemSpan );
}
CCPoint UIList::getRightHidenPosition(int itemId)
{
	return ccp(_viewWidth + _viewx,LIST_H);
}
CCPoint UIList::getShowPosition(int itemId)
{
	return ccp(_viewx,LIST_H);
}
CCPoint UIList::getShowMidPosition(int itemId)
{
	return ccp (_viewx + _viewWidth / 2 ,LIST_H);
}
CCPoint UIList::getLeftHidenPosition()
{
	return ccp(_viewx - _viewWidth ,0);
}
CCPoint UIList::getUpHidenPosition()
{
	return ccp(_viewx,_viewy + _viewHeight);
}

void UIList::moveEnded()
{
	if (nowId < _items.size() && nowId - startId < getHeightItemCount())
	{
		UIItem * item = _items.at(nowId);
		if (item)
		{
			CCPoint pos = getShowPosition(item->bagId - startId);
			CCMoveTo *action = CCMoveTo::create(0.1f,pos);
			item->runAction(CCSequence::create(action,NULL));
		}
	}
	nowId++;
	if (nowId < _items.size() && nowId - startId < getHeightItemCount())
	{
		UIItem * item = _items.at(nowId);
		if (item)
		{
			item->show();
			CCPoint pos = getShowMidPosition(item->bagId - startId);
			CCMoveTo *action = CCMoveTo::create(0.1f,pos);
			item->runAction(CCSequence::create(action,
					CCCallFunc::create(this, callfunc_selector(UIList::moveEnded)),NULL));
		}
	}
}

/** 
 * ����Ƿ���������
 */
bool UIList::touchDown(float x,float y)
{
	if (!this->isVisible()) return false;
	CCPoint pos = ccp(x,y);
	pos = this->convertToNodeSpace(pos);
	nowDragItemId = -1;
	touchIn = false;
	nowTouchPoint = ccp(x,y);
	if ( pos.x >= _viewx && pos.y >= _viewy && pos.x <= _viewx + _viewWidth && pos.y <= _viewy + _viewHeight)
	{
		if (!editable)
		{
			nowDragItemId = (_viewy + _viewHeight - pos.y) / (_eachItemHeight + _downItemSpan) + startId;
		}
		touchIn = true;
		return true;
	}
	return false;
}
/**
* ����λ��
*/
void  UIList::setPosition(float x,float y)
{
	CCNode::setPosition(ccp(x,y));
	this->x = x;
	this->y = y;
}
/**
 * ����λ��
 */
bool UIList::touchMove(float x,float y)
{
	if (!this->isVisible()) return false;
	if (!touchIn) return false;
	CCPoint pos = ccp(x,y);
//	pos = this->convertToNodeSpace(pos);
	if (editable)
	{
		CCPoint nowPoint = getPosition();
        setPosition(nowPoint.x + pos.x - nowTouchPoint.x,
                                      nowPoint.y + pos.y - nowTouchPoint.y);
		nowTouchPoint = pos;
	}
	else
	{
		int up = -1;
        int dt = pos.y - nowTouchPoint.y;
		
		if (dt < -(_eachItemHeight + _downItemSpan) )
		{
			up = dt / (_eachItemHeight + _downItemSpan);
		}
		else if (dt >= (_eachItemHeight + _downItemSpan))
		{
			up = dt / (_eachItemHeight + _downItemSpan);
		}
		else{
			return true;
		}
		nowDragItemId = -1;
		nowTouchPoint = pos;
		//if (nowDragItemId < _items.size() && nowDragItemId >=0 )
		{
			if (startId > _items.size() && up > 0) return true;
			if (up > 0) {startId += up;}
			if (startId <= 0 && up < 0) return true;
			if (up < 0) {startId += up;};
			// ���Ե��� ���� ����startId
			showEach();
		}
	}
	return true;
}
UIItem * UIList::getNowItem()
{
	if (nowDragItemId >= 0 && nowDragItemId < _items.size())
	{
		return _items.at(nowDragItemId);
	}
	return NULL;
}
void UIList::execEach(stExecListItem *exec)
{
	if (!exec) return;
	for ( ITEMS_ITER iter = _items.begin(); iter != _items.end();++iter)
	{
		exec->exec(*iter);
	}
}
/**
 * ֹͣ�϶�
 */
bool UIList::touchEnd(float x,float y)
{
	if (!this->isVisible()) return false;
	CCPoint pos = ccp(x,y);
	if (!editable && touchIn)
	{
		if (nowDragItemId < _items.size() && nowDragItemId >=0)
		{
			UIItem * item = _items.at(nowDragItemId);
			if (item)
			{
				// ���������¼�
				doItemDown(item);
				return true;
			}
		}
		nowTouchPoint = pos;
	}
	nowDragItemId = -1;
	touchIn = false;
	return true;
}
void UIList::doItemDown(UIItem *item)
{
	this->doEvent(UIBase::EVENT_CLICK_DOWN,this);
}
NS_CC_END