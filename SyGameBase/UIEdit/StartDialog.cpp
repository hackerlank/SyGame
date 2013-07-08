#include "StartDialog.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
NS_CC_BEGIN
void StartDialog::doInitEvent()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	do{
		GET_UI_BYNAME(this,UIButton,reqLoginBtn,"login");
		if (reqLoginBtn)
		{
			struct stReqLogin:public UICallback{
				void callback(UIBase *base)
				{
					// ���Զ�ȡϵͳ�е� ��¼�� ��¼���� �����͵�����
					// ��Request.cpp �д�������
					// �����ȴ�����
					
					GET_UI_BYNAME(dialog,UIEditField,uiName,"name");
					std::string userName,userPwd;
					if (uiName)
					{
						userName = uiName->getContent();
					}
					GET_UI_BYNAME(dialog,UIEditField,pwdName,"pwd");
					if (pwdName)
					{
						userPwd = pwdName->getContent();
					}
				}
				stReqLogin(UIWindow *window,StartDialog *dialog):window(window),dialog(dialog)
				{
				
				}
				UIWindow *window;
				StartDialog *dialog;
			};
		}
		
	} while (false);
}
/**
 * �رյ�ǰpanel
 **/
struct stClosePanel:public UICallback{
	void callback(UIBase *base)
	{
		if (base && base->getParent())
		{
			base->getParent()->setVisible(false);
		}
	}
	stClosePanel()
	{
	
	}
};
/**
 * չʾ����
 */
struct stShowProp:public UICallback{
public:
	void callback(UIBase *base)
	{
	
	}

};
/**
 * �༭��ϢDown
 */
struct stEditDown:public UICallback{
public:
	virtual  void callback(UIBase *base)
	{
		PropDialog *propPanel = (PropDialog*)window->getPanel("prop");
		if (propPanel)
		{
			propPanel->show(base);
		}
	}
	stEditDown(MainDialog *dialog,UIWindow *window): dialog(dialog),window(window)
	{}
protected:
	MainDialog *dialog;
	UIWindow *window;
};
/**
 * ������ť
 */
struct stCreateButton:public UICallback{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			UIPanel *panel = window->getNowTouchPanel();
			if (panel)
			{
				UIButton *btn = (UIButton*)panel->createButton(ccp(0,300),CCSizeMake(64,64),
					"btn_common_down.png","btn_common_up.png",panel);
				if (btn)
				{
					btn->setEditable(true);
					btn->bind(UIBase::EVENT_EDIT_DOWN,new stEditDown(dialog,window));
				}
			}
		}
	}
	stCreateButton(MainDialog *dialog,UIWindow *window):dialog(dialog),window(window)
	{
	
	}
protected:
	MainDialog *dialog;
	UIWindow *window;
};

/**
 * ����Panel
 */
struct stCreatePanel:public stCreateButton{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			DefaultDialog * ddialog = DefaultDialog::create(window,"defaultdialog.xml");
			if (ddialog)
			{
				ddialog->bind(UIBase::EVENT_EDIT_DOWN,new stEditDown(dialog,window));
			}
		}
	}
	stCreatePanel(MainDialog *dialog,UIWindow *window):stCreateButton(dialog,window)
	{}
};
/**
 * ����Panel
 */
struct stSavePanel:public stCreateButton{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			SavePanelDialog *savePanel = (SavePanelDialog*)window->getPanel("save");
			if (savePanel)
			{
			savePanel->nowPanel = window->getNowTouchPanel();
			savePanel->setVisible(true);
			}
		}
	}
	stSavePanel(MainDialog *dialog,UIWindow *window):stCreateButton(dialog,window)
	{}
};
/**
 * ����ͼƬ
 */
struct stCreateImage:public stCreateButton{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			// ����ͼƬ
			UIPanel *panel = window->getNowTouchPanel();
			if (panel)
			{
				UIImage *image = (UIImage*) panel->createImage(ccp(0,300),CCSizeMake(0,0),"defaultimg.png",panel);
				if (image)
				{
					image->setEditable(true);
					image->bind(UIBase::EVENT_EDIT_DOWN,new stShowProp());
				}
			}
		}
	}
	stCreateImage(MainDialog *dialog,UIWindow *window):stCreateButton(dialog,window)
	{}
};
/**
 * �����ı�
 */
struct stCreateText:public stCreateButton{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			// ����ͼƬ
			UIPanel *panel = window->getNowTouchPanel();
			if (panel)
			{
				UILabel * label = (UILabel*)panel->createLabel(ccp(0,0),CCSizeMake(0,0),"default",32,panel);
				if (label)
				{
					label->setEditable(true);
					label->bind(UIBase::EVENT_EDIT_DOWN,new stShowProp());
				}
			}
		}
	}
	stCreateText(MainDialog *dialog,UIWindow *window):stCreateButton(dialog,window)
	{
		
	}
};

/**
 * ���������
 */
struct stCreateEditField :public stCreateButton{
public:
	virtual void callback(UIBase *base)
	{
		if (dialog && window)
		{
			// ����ͼƬ
			UIPanel *panel = window->getNowTouchPanel();
			if (panel)
			{
				UIEditField *editField = (UIEditField*) panel->createTextField(ccp(0,0),CCSizeMake(100,32),"hello,world",panel);
				if (editField)
				{
					editField->setEditable(true);
					editField->bind(UIBase::EVENT_EDIT_DOWN,new stShowProp());
				}
			}
		}
	}
	stCreateEditField(MainDialog *dialog,UIWindow *window):stCreateButton(dialog,window)
	{}
};

/**
 * ��������
 */



/**
 * ��������
 */

/**
 * ��������BAG
 */

/**
 * �������涯��
 */

void MainDialog::doInitEvent()
{	
	do{
		GET_UI_BYNAME(this,UIButton,closeBtn,"close");
		if (closeBtn)
		{
			closeBtn->bind(UIBase::EVENT_CLICK_DOWN,new stClosePanel());
		}
		GET_UI_BYNAME(this,UIButton,crtBtnBtn,"button");
		if (crtBtnBtn)
		{
			crtBtnBtn->bind(UIBase::EVENT_CLICK_DOWN,new stCreateButton(this,this->window));
		}
		GET_UI_BYNAME(this,UIButton,crtImageBtn,"image");
		if (crtImageBtn)
		{
			crtImageBtn->bind(UIBase::EVENT_CLICK_DOWN,new stCreateImage(this,this->window));
			crtImageBtn->bind(UIBase::EVENT_EDIT_DOWN,new stEditDown(this,this->window));
		}
		GET_UI_BYNAME(this,UIButton,crtTextBtn,"text");
		if (crtTextBtn)
		{
			crtTextBtn->bind(UIBase::EVENT_CLICK_DOWN,new stCreateText(this,this->window));
		}
		GET_UI_BYNAME(this,UIButton,crtEditBtn,"editfield");
		if (crtEditBtn)
		{
			crtEditBtn->bind(UIBase::EVENT_CLICK_DOWN,new stCreateEditField(this,this->window));
		}
		GET_UI_BYNAME(this,UIButton,crtPanelBtn,"panel");
		if (crtPanelBtn)
		{
			crtPanelBtn->bind(UIBase::EVENT_CLICK_DOWN,new stCreatePanel(this,this->window));
		}
		GET_UI_BYNAME(this,UIButton,saveBtn,"save");
		if (saveBtn)
		{
			saveBtn->bind(UIBase::EVENT_CLICK_DOWN,new stSavePanel(this,this->window));
		}
	}while(false);
}

/**
 * չʾ������ֵ
 */
void PropBaseShow::show(PropDialog *propDialog,UIBase *base)
{
	GET_UI_BYNAME(propDialog,UIEditField,ex,"x");
	if (ex)
	{
		std::stringstream xstr;
		xstr << base->x;
		ex->setContent(xstr.str().c_str());
	}
	GET_UI_BYNAME(propDialog,UIEditField,ey,"y");
	if (ey)
	{
		std::stringstream ystr;
		ystr << base->y;
		ey->setContent(ystr.str().c_str());
	}
	GET_UI_BYNAME(propDialog,UIEditField,ew,"w");
	if (ew)
	{
		std::stringstream wstr;
		wstr << base->w;
		ew->setContent(wstr.str().c_str());
	}
	GET_UI_BYNAME(propDialog,UIEditField,eh,"h");
	if (eh)
	{
		std::stringstream hstr;
		hstr << base->h;
		eh->setContent(hstr.str().c_str());
	}
}


/**
 * չʾButton������ �趨Ӧ��չʾ������
 */
void PropButtonShow::show(PropDialog *propDialog,UIButton *button)
{
	PropBaseShow::show(propDialog,button);
	GET_UI_BYNAME(propDialog,UILabel,lbtn_up_img,"1btn_up_name");
	if (lbtn_up_img)
	{
		lbtn_up_img->setVisible(true);
	}
	GET_UI_BYNAME(propDialog,UILabel,lbtn_down_img,"lbtn_down_name");
	if (lbtn_down_img)
	{
		lbtn_down_img->setVisible(true);
	}
	GET_UI_BYNAME(propDialog,UIEditField,ebtn_up_img,"btn_up_name");
	if (ebtn_up_img)
	{
		ebtn_up_img->setVisible(true);
	}
	GET_UI_BYNAME(propDialog,UIEditField,ebtn_down_img,"btn_down_name");
	if (ebtn_down_img)
	{
		ebtn_down_img->setVisible(true);
	}
}


void PropPanelShow::show(PropDialog* propDialog,UIPanel *panel)
{
	PropBaseShow::show(propDialog,panel);
}

/**
 * ��������
 */
void PropDialog::hideProps()
{
	
	GET_UI_BYNAME(this,UILabel,lbtn_up_img,"1btn_up_name");
	if (lbtn_up_img)
	{
		lbtn_up_img->setVisible(false);
	}
	GET_UI_BYNAME(this,UILabel,lbtn_down_img,"lbtn_down_name");
	if (lbtn_down_img)
	{
		lbtn_down_img->setVisible(false);
	}
	GET_UI_BYNAME(this,UIEditField,ebtn_up_img,"btn_up_name");
	if (ebtn_up_img)
	{
		ebtn_up_img->setVisible(false);
	}
	GET_UI_BYNAME(this,UIEditField,ebtn_down_img,"btn_down_name");
	if (ebtn_down_img)
	{
		ebtn_down_img->setVisible(false);
	}
}
/**
 * չʾ�����ؼ�
 */
void PropDialog::show(UIBase *base)
{
	hideProps();
	this->setVisible(true);
	if (base->uiType == UIBase::UI_BUTTON)
	{
		show((UIButton*) base);
	}
	if (base->uiType == UIBase::UI_PANEL)
	{
		show((UIPanel*)base);
	}
}
/**
 * չʾbutton ������
 */
void PropDialog::show(UIButton *button)
{
	PropButtonShow bs;
	bs.show(this,button);
}
/**
 * չʾpanel ������
 */
void PropDialog::show(UIPanel *panel)
{
	PropPanelShow ps;
	ps.show(this,panel);
}
/**
 * ���������Խ���
 */
void PropDialog::doInitEvent()
{
	GET_UI_BYNAME(this,UIButton,closeBtn,"close");
	if (closeBtn)
	{
		closeBtn->bind(UIBase::EVENT_CLICK_DOWN,new stClosePanel());
	}
}
struct stSurePanel:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (base && base->getParent())
		{
			base->getParent()->setVisible(false);
			GET_UI_BYNAME(saveDialog,UIEditField,txtField,"filename");
			if (txtField)
			{
				saveDialog->savePanel(txtField->getContent());
			}
		}
	}
	stSurePanel(SavePanelDialog *saveDialog):saveDialog(saveDialog)
	{
	
	}
	SavePanelDialog* saveDialog;
};
void SavePanelDialog::doInitEvent()
{
	do{
		GET_UI_BYNAME(this,UIButton,cancelBtn,"canel");
		if (cancelBtn)
		{
			cancelBtn->bind(UIBase::EVENT_CLICK_DOWN,new stClosePanel());
		}
		GET_UI_BYNAME(this,UIButton,sureBtn,"sure");
		if (sureBtn)
		{
			sureBtn->bind(UIBase::EVENT_CLICK_DOWN,new stSurePanel(this)); // ȷ������
		}
	}while(false);
}

void SavePanelDialog::savePanel(const std::string &name)
{
	if (nowPanel)
		nowPanel->makeXmlFile(name);
}
NS_CC_END
