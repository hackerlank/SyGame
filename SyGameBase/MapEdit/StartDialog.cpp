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
 * �������� ���붯������ ָ������֡��ͼƬ ��֡�������� ���Բ��Զ���
 */

/**
 * ����ͼƬ �����ļ��� �趨��С λ�� ���
 */

/**
 * ���������� ����������������� ����ڳ��� ���ƶ�
 */

/**
 * �½�һ����ͼ �趨����
 * <map width="" height="" name="">
 *	<cartoon name="" x="" y=""/>
 *	<img src="" x="" y="" w="" h=""/>
 *	<backgroud x="" y="" w="" h="" speed="" />
 * </map>
 */

void MainDialog::doInitEvent()
{	
	do{
		
	}while(false);
}

/**
 * չʾ������ֵ
 */
void PropBaseShow::show(PropDialog *propDialog,UIBase *base)
{
	
}


/**
 * չʾButton������ �趨Ӧ��չʾ������
 */
void PropButtonShow::show(PropDialog *propDialog,UIButton *button)
{
	
}


void PropPanelShow::show(PropDialog* propDialog,UIPanel *panel)
{

}

/**
 * ��������
 */
void PropDialog::hideProps()
{
	
}
/**
 * չʾ�����ؼ�
 */
void PropDialog::show(UIBase *base)
{
	
}
/**
 * չʾbutton ������
 */
void PropDialog::show(UIButton *button)
{
	
}
/**
 * չʾpanel ������
 */
void PropDialog::show(UIPanel *panel)
{
	
}
/**
 * ���������Խ���
 */
void PropDialog::doInitEvent()
{
	
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
