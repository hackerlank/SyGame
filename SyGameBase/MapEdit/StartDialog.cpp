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
	// TODO ����һ����ͼƬ
	GET_UI_BYNAME(this,UIButton,createBgImageBtn,"bigimage") // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (createBgImageBtn)
	{
		createBgImageBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::createBigImage));
	}
	// TODO �򿪵�ǰ��ͼ����
	GET_UI_BYNAME(this,UIButton,showMapBtn,"map") // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (showMapBtn)
	{
		showMapBtn->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::showMapProp));
	}
	GET_UI_BYNAME(this,UIChoice,editBlockChoice,"editblock") // ��һ����ͼ �����滻��ǰ�ĵ�ͼ
	if (editBlockChoice)
	{
		editBlockChoice->bind(UIBase::EVENT_CLICK_DOWN,ui_function(MainDialog::editBlock));
	}
	
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
			if (panel->isModel())
			{
				panel->getWindow()->popModel();
			}
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
				MapManager::getMe().replaceMap(map); // ���ӵ�ͼ
			}
			panel->setVisible(false); // ����������
			if (panel->isModel())
			{
				panel->getWindow()->popModel();
			}
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
	window->pushModel(panel);
	panel->bindBtnClick("open",new OpenMapLogic()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->setVisible(true);
}
void MainDialog::saveMap(UIBase *base)
{
	MapManager::getMe().mapRoot->save();
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
			cartoon->rebuild();
			MapManager::getMe().getMap()->addCartoon(cartoon);
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
		nowPanel->hide();
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
				window->pushModel(addFrame);
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
	window->pushModel(panel);
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
			if (panel->isModel())
			{
				panel->getWindow()->popModel();
			}
		}
	}
};
/**
 * ��Ӧ����ͼ��İ�ť
 */
void MainDialog::createImage(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("createimage");// ��openmap.xml ��Panel
	window->pushModel(panel);
	panel->bindBtnClick("ok",new CreateImageLogic()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->setVisible(true);
}
/**
 * ����ͼ��
 */
class CreateBigImage:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			MutiBigImage *image = MutiBigImage::create(PANEL(panel,"extinfo")->getEditFieldValue("pngname").c_str());
			// ��ȡ�����ֶε�ֵ
			int x = 0, y = 0, w = 0, h = 0;
			PANEL(panel,"baseinfo")->getEditFieldValue("x",x);
			PANEL(panel,"baseinfo")->getEditFieldValue("y",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("w",y);
			PANEL(panel,"baseinfo")->getEditFieldValue("h",y);
			image->setBaseInfo(x,y,w,h);
			// ��ȡlist ������Ϣ
			//image->rebuild();

			MapManager::getMe().getMap()->addBigImage(image);

			panel->setVisible(false);
			if (panel->isModel())
			{
				panel->getWindow()->popModel();
			}
		}
	}
};
/**
 * ��Ӧչʾ����������ť
 **/
void MainDialog::createBigImage(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("createimage");// ��openmap.xml ��Panel
	window->pushModel(panel);
	panel->bindBtnClick("ok",new CreateBigImage()); // �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->setVisible(true);
}
class MapInfoItem:public BaseUIItem<MapInfoItem>
{
public:
	std::string mapName;
	MutiMap *map;
	MapInfoItem()
	{
		map = NULL;
	}
};
/**
 * �༭��ǰ��Ŀ
 */
struct stEditItem:public UICallback{
public:
	void callback(UIBase *base)
	{
		MapManager::getMe().choiceMap(map);
	}
	stEditItem(MapInfoItem *item,MutiMap *map):item(item),map(map)
	{
		
	}
	MapInfoItem *item;
	MutiMap *map;
};
struct stShowItem:public UICallback{
public:
	void callback(UIBase *base)
	{
		 UIChoice *choice = (UIChoice*) base;
		 if (choice && choice->isChoiced())
		 {
			map->show();
		 }
		 else if (choice)
		 {
			map->hide();
		 }
	}
	stShowItem(MapInfoItem *item,MutiMap *map):item(item),map(map)
	{
		
	}
	MapInfoItem *item;
	MutiMap *map;
};
struct stListItemDown:public UICallback{
public:
	void callback(UIBase *base)
	{
		// չʾ��ϸ��Ϣ
	}
};

/**
 * �����µı���
 **/
struct stNewBackGroud:public UICallback{
public:
	void callback(UIBase *base)
	{
		MapInfoItem *item = MapInfoItem::create("bgiteminfo.xml");
		if (item)
		{
			UIPanel *targetPanel = window->getPanel("showmap");
			UIPanel *infoPanel = window->getPanel("bginfo");
			if (targetPanel && infoPanel)
			{
				LIST(PANEL(targetPanel,"extinfo"),"list")->addItem(item);
				
				
				LIST(PANEL(targetPanel,"extinfo"),"list")->show();
				LIST(PANEL(targetPanel,"extinfo"),"list")->bind(UIBase::EVENT_CLICK_DOWN,new stListItemDown());
				// ���������ĵ�ͼ������
				GET_UI_BYNAME(item->getPanel(),UILabel,bgName,"bgname");
				if (bgName)
				{
					bgName->setContent(EIDTFIELD(infoPanel,"bgname")->getContent().c_str()); // ��������
				}
				// ����һ����ͼ
				MutiMap *muMap = MutiMap::create("defaultbg.xml");
				if (muMap && MapManager::getMe().getMap())
				{
					MapManager::getMe().getMap()->addMap(muMap);
					MapManager::getMe().choiceMap(muMap); // ��ǰ��ͼΪ�༭����
				}
				item->getPanel()->bindBtnClick("edit",new stEditItem(item,muMap)); // ���¼�
				item->getPanel()->bindChoiceClick("show",new stShowItem(item,muMap)); // ���¼�
				item->mapName = bgName->getContent();
			}
		}
	}
	stNewBackGroud(UIWindow *window):window(window)
	{
	
	}
	UIWindow *window;
};
/**
 * չʾ��������չʾ���Խ���
 */
struct stShowBackgroud:public UICallback{
public:
	void callback(UIBase *base)
	{
		// չʾbg�����Խ���
		UIPanel *panel = window->showPanel("bginfo");// ��bginfo.xml ��Panel
		if (panel)
		{
			panel->bindBtnClick("ok",new stNewBackGroud(window));
			panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
		}
	}
	stShowBackgroud(UIWindow *window):window(window)
	{}
	UIWindow *window;
};
struct stHadQqualItem:public stBagExecEach{
	void exec(UIItem *item) 
	{
		MapInfoItem *infoItem = (MapInfoItem*) item;
		if (infoItem->mapName == mapName)
		{
			value = true;
		}
	}
	bool value;
	stHadQqualItem()
	{
		value = false;
	}
	std::string mapName;
};
/**
 * ���������ϵ�ÿ����ͼ
 */
struct stShowEachBg:public stExecEachBackgroud{
public:
	void exec(MutiMap *map)
	{
		MapInfoItem *item = MapInfoItem::create("bgiteminfo.xml");
		if (item)
		{
			UIPanel *targetPanel = window->getPanel("showmap");
			if (targetPanel)
			{
				// �ȱ����Ƿ������ͬ��
				UISuperBag * list = LIST(PANEL(targetPanel,"extinfo"),"list");
				stHadQqualItem exec;
				exec.mapName = map->fileName;
				list->execEachItem(&exec);
				if (!exec.value)
				{
					list->addItem(item);
					item->getPanel()->bindBtnClick("edit",new stEditItem(item,map)); // ���¼�
					item->getPanel()->bindChoiceClick("show",new stShowItem(item,map)); // ���¼�
				}
				GET_UI_BYNAME(item->getPanel(),UILabel,bgName,"bgname");
				if (bgName)
				{
					bgName->setContent(map->fileName.c_str()); // ��������
				}
				item->mapName = map->fileName;

				list->show();
			}
		}
	}
	UIWindow *window;
};

class SetPanelProp:public UICallback{
public:
	void callback(UIBase *base)
	{
		// �趨��ǰ��map������
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			MutiMap *map = MapManager::getMe().getMap();
			if (map)
			{
				PANEL(panel,"baseinfo")->getEditFieldValue("x",map->m_tPosition.x);
				PANEL(panel,"baseinfo")->getEditFieldValue("y",map->m_tPosition.y);
				map->setZOrder(atoi(PANEL(panel,"baseinfo")->getEditFieldValue("zorder").c_str())); // ����zorder ����
				std::string name = PANEL(panel,"baseinfo")->getEditFieldValue("name");
				if (name != "")
					map->fileName = name; 
			}
			panel->hide();
		}
	}
};

/**
 * ��Ӧչʾ���԰�ť
 */
void MainDialog::showMapProp(UIBase *base)
{
	UIWindow *window = getWindow();
	/**
	 * չʾһ��dialog Я����Ӧ��btn �Ĵ����¼�
	 */
	UIPanel *panel = window->showPanel("showmap");// ��showmap.xml ��Panel
//	window->pushModel(panel);
	// ���԰���صĴ����¼�
	PANEL(panel,"extinfo")->bindBtnClick("addbg",new stShowBackgroud(window)); // ����һ����
	panel->bindBtnClick("cancel",new CloseMe());// �󶨰�ť����Ӧ�¼�
	panel->bindBtnClick("ok",new SetPanelProp()); // �趨Panel ������
	PANEL(panel,"extinfo")->setVisible(false);
	panel->bindChoiceClick("basechoice",new stChoiceBasePanel());
	panel->bindChoiceClick("extchoice",new stChoiceExtPanel());
	panel->setVisible(true);
	stShowEachBg exec;
	exec.window = window;
	MapManager::getMe().mapRoot->execEachBg(&exec); // ˢ���б�
}

void MainDialog::editBlock(UIBase *base)
{
	UIChoice *choice = (UIChoice*) base;
	if (choice)
	{
		if (choice->isChoiced())
		{
			MapManager::getMe().isSetBlock = true;
		}
		else
		{
			MapManager::getMe().isSetBlock = false;
		}
	}
}
NS_CC_END
