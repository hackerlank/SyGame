#include "StartDialog.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
NS_CC_BEGIN


void MainDialog::doInitEvent()
{	
	GET_UI_BYNAME(this,UIButton,openBtn,"open"); // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (openBtn)
	{
		openBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::openMap));
	}
	// TODO ����һ���µĵ�ͼ

	// TODO ����һ��ͼƬ

	// TODO ����һ������

	// TODO ����һ������
}

/**
 * �ر��Լ�
 */
class CloseMe:public UICallback
{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			panel->setVisible(false);
		}
	}
};
/**
 * �򿪵�ͼ�߼�
 */
class OpenMapLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			std::string value = panel->getEditFieldValue("filename");
			// ����һ��map

			panel->setVisible(false); // ����������
		}
	}
	//CCScene *scene;
};
void MainDialog::openMap(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("openmap");// ��openmap.xml ��Panel
	panel->bindBtnClick("open",new OpenMapLogic()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->setVisible(true);
}
NS_CC_END
