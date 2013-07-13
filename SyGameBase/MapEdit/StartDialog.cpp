#include "StartDialog.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
#include "MapManager.h"
#include "MutiCartoon.h"
#include "MutiImage.h"
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
	GET_UI_BYNAME(this,UIButton,createImgBtn,"image"); // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (createImgBtn)
	{
		createImgBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::createImage));
	}
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
			MutiMap *map = MutiMap::create(value.c_str());
			if (map)
			{
				MapManager::getMe().addMap(map); // ���ӵ�ͼ
			}
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

/**
 * ֡����Ŀ
 */
struct stFrameItem:public UIItem{
public:
	// list�е�Ԫ��
	static stFrameItem *create(const std::string &pngName)
	{
		return NULL;
	}
	std::string pngName;
};
/**
 * ��ȡ������Ϣ
 */
struct stGetCartoonInfo:stBagExecEach{
	void exec(UIItem *item) 
	{
		stFrameItem *frameItem = (stFrameItem*)(item);
		if (frameItem && cartoon)
		{
			cartoon->pngNames.push_back(frameItem->pngName);
		}
	}
	MutiCartoon *cartoon;
	stGetCartoonInfo(MutiCartoon *cartoon):cartoon(cartoon)
	{
		
	}
};
/**
 * ���Դ�������
 */
class CreateCartoonLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			MutiCartoon *cartoon = MutiCartoon::create();
			// ��ȡ�����ֶε�ֵ
			int x = 0, y = 0, w = 0, h = 0;
			PANEL(panel,"baseinfo")->getEditFieldValue("x",x);
			PANEL(panel,"baseinfo")->getEditFieldValue("y",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("w",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("h",y);
			cartoon->setBaseInfo(x,y,w,h);

			// ��ȡlist ������Ϣ
			stGetCartoonInfo exec(cartoon);
			LIST(PANEL(panel,"extinfo"),"list")->execEachItem(&exec);
			PANEL(panel,"extinfo")->getEditFieldValue("taptime",cartoon->tapTime);
			PANEL(panel,"extinfo")->getEditFieldValue("repeatetimes",cartoon->repeateTimes);

		}
	}
};
/**
 * ��֡��ӽ�Panel
 */
struct stAddFrame:public UICallback{
public:
	void callback(UIBase *base)
	{
		// ��panel�л�ȡList Ȼ����ӽ�Ԫ��
		UIPanel *nowPanel = base->getPanel();
		stFrameItem *item = stFrameItem::create(nowPanel->getEditFieldValue("pngname"));
		LIST(panel,"list")->addItem(item);
		nowPanel->setVisible(false);
	}
	stAddFrame(UIWindow *window,UIPanel *panel):window(window),panel(panel)
	{
		
	}
	UIWindow *window;
	UIPanel *panel;
};
/**
 * �����Ի��� ������֡��Ϣ
 */
class stShowAddFrame:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			UIPanel *addFrame = window->showPanel("addframe"); // TODO ����֡
			if (addFrame)
			{
				addFrame->bindBtnClick("ok",new stAddFrame(window,panel));
			}
		}
	}
	stShowAddFrame(UIWindow *window):window(window)
	{}
	UIWindow *window;
};
/**
 * ��������
 */
void MainDialog::createCartoon(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("createcartoon");// ��openmap.xml ��Panel
	panel->bindBtnClick("ok",new CreateCartoonLogic()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�

	// ������frame�İ�ť
	PANEL(panel,"extinfo")->bindBtnClick("addframe",new stShowAddFrame(window));
	GET_UI_BYNAME(PANEL(panel,"extinfo"),UISuperBag,bag,"list");
	//bag->bind(); ����bag�ĵ����Ϣ
	panel->bindChoiceClick("basechoice",NULL);
	panel->bindChoiceClick("extchoice",NULL);
	panel->setVisible(true);
}

/**
 * ����ͼ��
 */
class CreateImageLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			MutiImage *image = MutiImage::create();
			// ��ȡ�����ֶε�ֵ
			int x = 0, y = 0, w = 0, h = 0;
			PANEL(panel,"baseinfo")->getEditFieldValue("x",x);
			PANEL(panel,"baseinfo")->getEditFieldValue("y",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("w",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("h",y);
			image->setBaseInfo(x,y,w,h);

			// ��ȡlist ������Ϣ
			image->pngName = PANEL(panel,"extinfo")->getEditFieldValue("pngname");
			image->rebuild();

			MapManager::getMe().getMap()->addImage(image);

			panel->setVisible(false);
		}
	}
};
void MainDialog::createImage(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("createimage");// ��openmap.xml ��Panel
	panel->bindBtnClick("ok",new CreateImageLogic()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->setVisible(true);
}
NS_CC_END
