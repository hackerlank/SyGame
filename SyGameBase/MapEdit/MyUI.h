#pragma once
#include "cocos2d.h"

USING_NS_CC;

namespace myui{
	enum TouchEvent{
		TOUCH_DOWN = 1,
		TOUCH_MOVE = 2,
		TOUCH_END = 3,
	}; // ����¼�
	enum AlignType{
		ALIGN_LEFT = 1 << 0,
		ALIGN_RIGHT = 1 << 1,
		ALIGN_UP = 1 << 2,
		ALIGN_BOTTOM = 1 << 3,
		ALIGN_CENTER = 1 << 4,
	}; // ���뷽ʽ
	class Base{
	public:
		Base()
		{
			touch = NULL;
			isActive = false;
			parent = NULL;
		}
		virtual bool checkIn(CCTouch *touch)
		{
			return false;
		}
		/**
		* ��Ӧtouch�¼�
		* ֧�Ŷ�㴥��
		*/
		virtual bool attachTouch(TouchEvent event,CCTouch *touch)
		{
			if (event == TOUCH_DOWN && !this->touch && checkIn(touch))
			{
				this->touch = touch;
				return doTouch(event,touch);
			}
			if (event == TOUCH_MOVE && touch == this->touch)
			{
				return doTouch(event,touch);
			}
			if (event == TOUCH_END && touch == this->touch)
			{
				bool tag =  doTouch(event,touch);
				this->touch = NULL;
				return tag;
			}
			return false;
		}
		virtual bool doTouch(TouchEvent event,CCTouch *touch)
		{
			return false;
		}
		CCTouch *touch;
		CCPoint nowTouchPoint;
		bool isActive;
		CCNode *parent;
		virtual void render(){}
	};

	/**
	 * 1.���Ը���ͼƬ����ȥ����Ƿ���ײ
	 * 2.���Զ����ƽ���ԭɫ
	 * 3.��Touch �к� ����Ԫ�ػ�������
	 * 4. ���ڼ̳��ڲ�����
	 */
	class Image:public Base{
	public:
		static Image *create(const char *pngName);
		/**
		 * ����Ƿ�����Touch
		 */
		bool checkIn(CCTouch *touch);
		/**
		* ���õ��Ƿ���ͼƬ��
		*/
		bool checkIn(const CCPoint &point);
		std::string pngName; // pngͼƬ
		std::string outlinePngName; // ����png����
		/**
		* ��������ͼƬ
		*/
		void setOutLine(const char *pngName);
		/**
		* ��Ӧtouch�¼�
		* ��Ҫ����ͼƬ���ƶ�
		*/
		bool doTouch(TouchEvent event,CCTouch *touch);

		/**
		* �ڲ�����Ԫ��
		*/
		CCSprite *sprite;

		void show(CCNode *node)
		{
			if (sprite)
				node->addChild(sprite);
		}
		/**
		* �滻��ǰͼƬ
		*/
		void replacePng(const char *name);
		bool checkIn(const CCPoint point,CCSprite *sprite,const char *fileName);
		bool checkInRect(const CCPoint point,CCSprite *sprite);
		bool createImage(const char *fileName,CCImage &image);

		Image()
		{
			sprite = NULL;
		}
		virtual void render()
		{
			if (sprite) sprite->visit();
		}
	};
	class Button:public Image{
	public:
		/**
		 * ������ť
		 */
		static Button * create(const char *upName,const char *downName);
		/**
		* ��Ӧtouch�¼�
		* ��Ҫ����ͼƬ���ƶ�
		*/
		bool doTouch(TouchEvent event,CCTouch *touch);
	
		std::string upName;
		std::string downName;
	};
	/**
	 * ��ȡxml ��ȡ�ı���Ϣ��չʾ
	 * ����������Ƶ�����ĵ�����
	 **/
	class Label:public Base{
	public:
		static Label * create(const char *content,const CCSize& size);
		/**
		 * �����ı���������չʾ����
		 */
		bool init(const char* content,const CCSize &size);
	};
	/**
	 * û�취 �ⶫ���÷��볡���� �ͼ̳�CCNode �� 
	 * 1.֧�ֶ�㴥���¼��ķַ� ��Ҫ������㴥�ػ���
	 * 2.��Ҫ������Ⱦ����
	 * 3.���ֹ���
	 *	 1.�µ����� ÿ����Ԫ��������
	 *		�㷨˵��: ÿ����Ԫ���Զ����Լ���ϸ�ָ��� �� ��ϸ�������ϵ�λ��
	 * 4.��Panel ���н�� Ĭ��һ����ɫ��ͼƬ
	 **/
	class Panel:public CCNode,public Base{
	public:
		std::list<Base*> uis;  // ����Ԫ���б� ��Base ������ʱ ����չʾ
		std::list<Base*> actives; // �����ui�б�
		std::list<Base*> firsts; // ���ȼ����б� ʵ��ģʽ��
		typedef std::list<Base*>::iterator BASES_ITER;
		/**
		* ��Ӧtouch�¼�
		* �������Base ����actives
		*/
		bool attachTouch(TouchEvent event,CCTouch *touch);
		/**
		 * ���ӽ���Ԫ��
		 */
		void addUI(Base *base);
		/**
		 * ��Ⱦ
		 */
		void visit();
		virtual void render()
		{
			visit();
		}
		/**
		 * ���������λ��
		 */
		void setLocation(AlignType alignType,const CCSize &splitSize,const CCPoint &gridLocation);
		/**
		 * ���ô�С
		 */
		void setSize(const CCSize &size);
		CCRect rect; // �������ڵ�����
		CCSize getSize(); // ��ȡ��С
		/**
		 * ��ȡ��ǰ���ֲ�����λ��
		 */
		CCPoint getPoint(AlignType alignType,const CCSize &splitSize,const CCPoint &gridLocation);
	};
	/**
	 * ����
	 */
	class Window:public CCLayer{
	public:
		static Window*create();
		bool init();
		void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
		void ccTouchesMoved(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
		void ccTouchesEnded(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
		std::list<Base*> uis;
		typedef std::list<Base*>::iterator UIS_ITER;
		std::list<Base*> actives; // �����ui�б�
		std::list<Base*> firsts; // ���ȼ����б� ʵ��ģʽ��
		/**
		 * ���ӽ���Ԫ��
		 */
		void addUI(Base *base);
		 /**
		  * ��Ⱦ
		  */
		void visit();
		/**
		 * ��ȡ���ڴ�С
		 */
		static CCSize getWindowSize();
		/**
		 * ��ȡ�����Ϣ
		 * \param clignType ���뷽ʽ
		 * \spliteSize ���ָ��
		 * \location ���ӱ��
		 */
		static CCPoint getPoint(AlignType alignType,const CCSize &splitSize,const CCPoint &gridLocation);
	};
};