#pragma once
#include "serialize.h"
#include "vector"
#include <string>
#include "cocos2d.h"
NS_CC_BEGIN
class UIBase;
typedef void (UIBase::*UI_EVENT_HANDLE)(UIBase*);
#define ui_function(_SELECTOR) FunctionInfo((UI_EVENT_HANDLE)(&_SELECTOR),this)

#define ui_object_function(object,_SELECTOR) FunctionInfo((UI_EVENT_HANDLE)(&_SELECTOR),object)
#define REGISTER_EVENT 
class UIItem;
struct stBagExecEach{
	virtual  void exec(UIItem *item) = 0;
};
#define GET_UI_BYID(Panel,Class,Fieldname,UniqueId)\
	Class *Fieldname = static_cast<Class*>( Panel->getUIById(UniqueId));
#define GET_UI_BYNAME(Panel,Class,Fieldname,UniqueName)\
	Class *Fieldname = static_cast<Class*>( Panel->getUIByName(UniqueName));

struct FunctionInfo{
	UI_EVENT_HANDLE handle;
	UIBase *object;
	FunctionInfo()
	{
		object = NULL;
		handle = NULL;
	}
	FunctionInfo(UI_EVENT_HANDLE handle,UIBase *object)
	{
		this->handle = handle;
		this->object = object;
	}
};
struct UICallback{
	virtual void callback(UIBase *base) = 0;
};
/**
 * UI �Ļ�����
 */
class UIBase:public CCNode,public serialize::Object{
public:
	int uniqueId; // Ψһ���
	std::string name; // �ؼ�����
	enum{
		UI_LABEL = 0, // ��̬�ı���
		UI_IMAGE_BUTTON = 1, // ͼ��ť
		UI_BUTTON = 2, // ��ť
		UI_LIST = 3, // �б�
		UI_CHOICE = 4, // ѡ��
		UI_BAG = 5, // ����
		UI_TEXT_FIELD = 6, // �����
		UI_RICH_TEXT = 7, // ���ı���
		UI_VALUE = 8, // ��ֵ��
		UI_PANEL = 9, // ����
	};
//	SERIALIZE_KIND(0);
	int uiType; // �ؼ�����
	int eventPriority;// �¼����ȼ�
	enum{
		EVENT_NULL = 0, // ���¼�
		EVENT_CLICK_DOWN = 1, // ĳ����Ŀ������
		EVENT_EDIT_DOWN = 2, // ĳ����Ŀ���༭
		EVENT_PANEL_LOAD = 3, // panel�������
		EVENT_DRAG_OUT = 4, //���屻ק��
		EVENT_TOUCH_IN, // ����
		EVENT_TOUCH_MOVE, // �������ƶ�
		EVENT_TOUCH_OUT, // ����
		EVENT_TOUCH_NO_CHILD_IN,
	};
	/**
	 * ����һ��������ui ���ڳ־û�
	 */
	static UIBase * create(int kind);
	/**
	 * ���¼��������
	 */
	virtual bool bind(int uiEvent,const FunctionInfo& function);
	virtual bool bind(int uiEvnet,UICallback *callback);
	virtual void doEvent(int uiEvent,UIBase *base);
	
	UIBase()
	{
		eventPriority = 0;
		uniqueId = 0;
		x = y = w = h = 0;
		uiType = -1;
	}
	~UIBase()
	{
		for (UI_CALLBACK_ITER iter = event_callbacks.begin(); iter != event_callbacks.end();++iter)
		{
			if(*iter) delete *iter;
		}
	}
	std::vector<FunctionInfo> event_functions;
	std::vector<UICallback*> event_callbacks;
	typedef std::vector<FunctionInfo>::iterator EVENT_FUNCTIONS_ITER;
	typedef std::vector<UICallback*>::iterator UI_CALLBACK_ITER;
	/**
	 * ����λ��
	 */
	virtual void setPosition(float x,float y) {return;}
	enum{
		TOUCH_DOWN = 0, // ����
		TOUCH_MOVE = 1, // �ƶ�
		TOUCH_END = 2, // ����
	};
	/**
	 * ������
	 * \parma touchType �������
	 * \param touchPoint �����
	 */
	virtual bool doTouch(int touchType,const CCPoint &touchPoint);
	/**
	 * ���ô�С
	 */
	virtual void setSize(float w,float h) {return;}
	 /** 
	 * ����Ƿ���������
	 */
	virtual bool touchDown(float x,float y) {return false;}
	/**
	 * ����λ��
	 */
	virtual bool touchMove(float x,float y){return false;}
	/**
	 * ֹͣ�϶�
	 */
	virtual bool touchEnd(float x,float y) {return false;}

	/**
	 * ����Ϊ�ɱ༭ģʽ
	 */
	virtual void setEditable(bool tag) {return ;}
	/**
	 * ���� �ɼ�
	 */
	void setVisible(bool tag);

	virtual void draw(){
		CCNode::draw();
	}

	virtual  void visit()
	{
		CCNode::visit();
	}
	static std::string toUTF8(const std::wstring& ); 
public:
	float x;
	float y;
	float w;
	float h;

	/**
	 * ���л�����
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
	}
};

NS_CC_END
