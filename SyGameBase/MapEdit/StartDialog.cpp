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
	GET_UI_BYNAME(this,UIButton,saveBtn,"save"); // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (saveBtn)
	{
		saveBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::saveMap));
	}
	// TODO ����һ���µĵ�ͼ

	// TODO ����һ��ͼƬ
	GET_UI_BYNAME(this,UIButton,createImgBtn,"image"); // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (createImgBtn)
	{
		createImgBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::createImage));
	}
	// TODO ����һ������
	GET_UI_BYNAME(this,UIButton,createCartoonBtn,"cartoon") // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (createCartoonBtn)
	{
		createCartoonBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::createCartoon));
	}
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
void MainDialog::saveMap(UIBase *base)
{
	MapManager::getMe().getMap()->save();
}
/**
 * ֡����Ŀ
 */
struct stFrameItem:public UIItem{
public:
	// list�е�Ԫ��
	static stFrameItem *create(const std::string &pngName)
	{
		stFrameItem *node = new stFrameItem();
		node->autorelease();
		node->init(pngName);
		return node;
	}
	void init(const std::string &pngName)
	{
		this->pngName = pngName;
		UILabel *info = UILabel::create(pngName.c_str(),32);
		if (info)
		{
			this->addChild(info);
			info->setPosition(84,25);
		}
		sprite = CCSprite::create(pngName.c_str());
		if (sprite)
		{
			addChild(sprite);
			sprite->setAnchorPoint(ccp(0,0));
		}
	}
	virtual void setSize(float w,float h)
	{
		if (sprite)
		{
			sprite->setScaleX(64/sprite->getContentSize().width);
			sprite->setScaleY(h / sprite->getContentSize().height);
		}
	}
	CCSprite *sprite;
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
		LIST(panel,"list")->show();
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
				addFrame->bindBtnClick("cancel",new CloseMe());
				addFrame->setVisible(true);
			}
		}
	}
	stShowAddFrame(UIWindow *window):window(window)
	{}
	UIWindow *window;
};
class stChoiceBasePanel:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		UIChoice *choice = (UIChoice*) base;
		PANEL(panel,"extinfo")->setVisible(false);
		PANEL(panel,"baseinfo")->setVisible(false);
		if (panel && !choice->isChoiced())
		{
			PANEL(panel,"baseinfo")->setVisible(true);
			CHOICE(panel,"extchoice")->setChoiced(false);
		}
		else if (panel)
		{
			PANEL(panel,"extinfo")->setVisible(true);
		}
	}
};
class stChoiceExtPanel:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		UIChoice *choice = (UIChoice*) base;
		PANEL(panel,"extinfo")->setVisible(false);
		PANEL(panel,"baseinfo")->setVisible(false);
		if (panel && !choice->isChoiced())
		{
			PANEL(panel,"extinfo")->setVisible(true);
			CHOICE(panel,"basechoice")->setChoiced(false);
		}
		else if(panel)
		{
			PANEL(panel,"baseinfo")->setVisible(true);
		}
	}
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
	PANEL(panel,"extinfo")->setVisible(false);
	GET_UI_BYNAME(PANEL(panel,"extinfo"),UISuperBag,bag,"list");
	//bag->bind(); ����bag�ĵ����Ϣ
	panel->bindChoiceClick("basechoice",new stChoiceBasePanel());
	panel->bindChoiceClick("extchoice",new stChoiceExtPanel());
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
