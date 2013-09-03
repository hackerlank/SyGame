#pragma once
#include "cocos2d.h"
#include "behavior.h"
#include "UIImage.h"
NS_CC_BEGIN
class stLineInfo{
	public:
		DebugActionShow* start;
		DebugActionShow* end;
	};
	class DebugShow;
    class LineLayer:public CCLayer{
	public:
		CREATE_FUNC(LineLayer);
		std::list<stLineInfo> lines;
		void draw()
		{
			CHECK_GL_ERROR_DEBUG();
			for (std::list<stLineInfo>::iterator iter = lines.begin();iter != lines.end();++iter)
			{
				ccDrawLine(ccp(iter->start->__node__->__debug__x__,iter->start->__node__->__debug__y__),
					ccp(iter->end->__node__->__debug__x__,iter->end->__node__->__debug__y__));
			}
		    
			// restore original values
			glLineWidth(1);
			ccDrawColor4B(255,255,255,255);
			ccPointSize(1);
		    
			CHECK_GL_ERROR_DEBUG();
		}
		void setLine(DebugActionShow * start,DebugActionShow *end)
		{
			stLineInfo line;
			line.start = start;
			line.end = end;
			lines.push_back(line);
		}
		void clearAll()
		{
			lines.clear();
		}
		void addChild(DebugShow *show);
		std::vector<DebugShow*> showes;

		bool doTouch(int touchType,const CCPoint &point);
	};
    class DebugShow:public DebugActionShow,public UIImage{
	public:
		static DebugShow*create(Node *node)
		{
			// ����3�����͵Ľڵ�
			
			DebugShow *show = new DebugShow();
			if (show->init("cell.png"))
			{
				show->autorelease();
				return show;
			}
			return NULL;
		}
		virtual void actionWithDebug(Node *node) // ÿ��ִ��ʱ�ص�
		{
			// �ı���ɫ
			//setColor(ccc3(10,255,0));
		}
		virtual void setPosition(int x,int y) // ����λ��
		{
			setPosition(ccp(x,y));
		}
		void setPosition(float x,float y)
		{
			setPosition(ccp(x,y));
		}
		void setPosition(const CCPoint &point)
		{
			UIImage::setPosition(point.x,point.y);
			if(DebugActionShow::__node__)
			{
				DebugActionShow::__node__->__debug__x__ = point.x;
				DebugActionShow::__node__->__debug__y__ = point.y;
			}
		}
		LineLayer *getPaint()
		{
			return (LineLayer * ) this->getParent();
		}
		virtual void showRelation(DebugActionShow *show) // չʾ����
		{
			// ����
			getPaint()->setLine(this,show);
		}
	};
	
NS_CC_END