/**
 * ������ ���Գ��� �������� ���ҿ���ӵ���ƶ�����
 */
#pragma once
#include "cocos2d.h"
#include "UIBase.h"
#include "xmlScript.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UINumber.h"
#include "UIImage.h"
#include "UIEditField.h"

NS_CC_BEGIN
class UIWindow;
struct stExecPanelEach{
	virtual void exec(UIBase *base) = 0;
};
class UIPanel:public UIBase{
public:
/**
 * ������������ ʹ�ú���
 * UIButton * btn  = this->createButton();
 * this->addUI(btn);
 * btn->bind(EVENT_CLICK,ui_function(&doClick));
 * function doClick(UIBase *button)
 * {
 *		for (UIItem* item : choiceList.content)
 *			for (int i = 0; i < 8;i++) model:setFrame(dir,i,imgName);
 * }
 **/
	/**
	* ����Button
	*/
	static UIBase * createButton(const CCPoint &position,const CCSize &size,const char * upImgName,const char *downImgName,UIPanel *parent = NULL,int uniqueId = -1);
	static UIBase * createButton(const CCPoint &position,const CCSize &size,const char* buttonName,UIPanel *parent = NULL,int uniqueId = -1);

	/**
	 * ����label
	 **/
	static UIBase *createLabel(const CCPoint &position,const CCSize &size,const char *content,float fontSize,UIPanel *parent = NULL,int uniqueId = -1);

	/**
	 * ����textfield
	 */
	static UIBase *createTextField(const CCPoint& position,const CCSize &size,const char* defaultContent,UIPanel *parent = NULL,int uniqueId = -1);

	/**
	 * ����ͼ��
	 */
	static UIBase *createImage(const CCPoint& position,const CCSize &size,const char *pngName,UIPanel *parent = NULL,int uniqueId = -1);

	/**
	 * ����ChoiceList
	 */
	static UIBase* createChoiceList(const CCPoint &position,const CCRect &size,const CCSize &eachSize,int startId,UIPanel *parent = NULL,int uniqueId = -1);
	
	/**
	 * ����List
	 */
	static UIBase* createList(const CCPoint &position,const CCRect &size,const CCSize &eachSize,int startId,UIPanel *parent = NULL,int uniqueId = -1);
	/**
	 * ����ѡ���
	 */
	static UIBase *createChoice(const CCPoint &position,const CCSize &size,const char * upImgName,const char *downImgName,UIPanel *parent = NULL,int uniqueId = -1);

	/**
	 * ����Bag
	 */
	static UIBase *createBag(const CCPoint &position,const CCSize & bagContent,const CCSize &eachBag,int eachLeftSpan,int eachUpSpan,UIPanel *parent = NULL,int uniqueId = -1);
	
	/**
	 * ����lineValue
	 */
	static UIBase *createLineValue(const CCPoint &position,float minValue,float maxValue,float defaultValue,UIPanel * parent = NULL,int uniqueId = -1);
	/**
	 * ����editģʽ
	 */
	void setEditable();

	void addUI(UIBase *base);
	
	bool addGloabUI(UIBase *base);

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
	virtual void setPosition(float x,float y) ;
	
	/**
	 * ���ô�С
	 */
	virtual void setSize(float w,float h) ;
	/**
	 * ��ȡUIBase
	 */
	UIBase *getUIById(int id);

	void setEditable(bool tag);
	static UIPanel* create();

	static UIPanel * createFromNode(script::tixmlCodeNode *node);
	bool initXFromNode(script::tixmlCodeNode *node);
	UIPanel()
	{
		_moveable = true;
		_width = 500;
		_height = 500;
		_touchIn = false;
		back = NULL;
		//back = CCSprite::create("panel_back.png");
		//if (back)
		//	this->addChild(back);
		_nowTouchUI = NULL;
		uiType = UIBase::UI_PANEL;
		_onlyMe = false; // ֻ��Ӧ�Լ���ʱ��
	}
	void replaceBack(const char *pngName);
	void  setMovable(bool tag)
	{
		_moveable = tag;
	}
	/**
	 * ���л�
	 */
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(x,0);
		SERIALIZE_DATA(y,1);
		SERIALIZE_DATA(w,2);
		SERIALIZE_DATA(h,3);
		SERIALIZE_DATA(uniqueId,4);
		SERIALIZE_DATA(name,5);
		SERIALIZE_DATA(uiType,6);
		SERIALIZE_DATA(_width,7);
		SERIALIZE_DATA(_height,8);
		SERIALIZE_DATA_CONDITION(childuis,9,childuis.size());
	}
	//SERIALIZE_KIND(UIBase::UI_PANEL);

	void beLoaded();
	void loadFromFile(const char *fileName);

	void saveToFile(const char *fileName);

	void execEach(stExecPanelEach *exec);
	/**
	 * չʾ�Լ��� ֻ��Ӧ�����¼�
	 */
	void beginModel(){_onlyMe = true;}
	bool isModel(){return _onlyMe;}
	void endModel(){_onlyMe = false;}
	UIBase *getUIByName(std::string name);
	void removeBack()
	{
		if (back)
		{
			this->removeChild(back,true);
			back = NULL;
		}
	}
	UIButton * createBtn(script::tixmlCodeNode &btnNode);
	UILabel * createLblFromNode(script::tixmlCodeNode * node);
	UINumber *createNumFromNode(script::tixmlCodeNode *node);
    UIImage * createImgFromNode(script::tixmlCodeNode *node);
    UIEditField *createFieldFromNode(script::tixmlCodeNode *node);
	void showByAction(int actionId);
protected:
	CCPoint nowTouchPoint;
	bool _moveable;
	bool _touchIn;
	std::vector<UIBase*> childuis;
	typedef std::vector<UIBase*>::iterator CHILD_UIS_ITER;
	/**
	 * ϵͳ�Ļ������漯��
	 */
	std::vector<UIBase*> uis;
	std::map<std::string,UIBase *> nameuis;
	typedef std::map<std::string,UIBase*>::iterator NAME_UIS_ITER;
	float _width;
	float _height;
	CCSprite *back;
	UIBase *_nowTouchUI;
	bool _onlyMe;

	static int const ALIGN_TOP = 1 << 0;
	static int const ALIGN_X_CENTER = 1 << 1;
	static int const ALIGN_DOWN = 1 << 2;
	static int const ALIGN_RIGHT = 1 << 3;
	static int const ALIGN_LEFT = 1 << 4;
	static int const ALIGN_Y_CENTER = 1 << 5;
};

template<class CHILD>
class BaseDialog:public script::tixmlCode,public UIPanel{
public:
	static CHILD* create(UIWindow *window,const char *uixml)
	{
		CHILD *node = new CHILD();
		if (node)
		{
			node->window = window;
			node->initFromFile(uixml);
			node->autorelease();
			return node;
		}
		CC_SAFE_DELETE(node);
		return NULL;
	}
	bool initFromFile(const char *fileName)
	{
		std::string startui = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName);
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
 			// ����Label TextArea Button ���ɽ���ϵͳ
			// ������ӭ����
			script::tixmlCodeNode mainNode = node->getFirstChildNode("start");
			if (mainNode.isValid())
			{
				this->initXFromNode(&mainNode);
				window->addPanel(this);
				this->setZOrder(12);
			}
		}
		vTakeNode(node);
	}
	virtual void vTakeNode(script::tixmlCodeNode *node){}
	BaseDialog()
	{
		window = NULL;
	}
	UIWindow *window;
protected:
	virtual void doInitEvent(){}
};

NS_CC_END