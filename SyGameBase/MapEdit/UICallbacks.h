#pragma once

#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
#include "MapManager.h"
#include "MutiCartoon.h"
#include "MutiImage.h"
#include "MutiMonster.h"
#include "UILib.h"
#include "FileSet.h"
NS_CC_BEGIN

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
			std::string fileName = panel->getUILablevalue("filename");

			// ��ȡlist ������Ϣ
			image->pngName = fileName;
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
 * ������ͼ�߼�
 */
class CreateBigImageLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel *target = base->getPanel();
			std::string fileName = target->getUILablevalue("filename");
			MutiBigImage *image = MutiBigImage::create(fileName.c_str());

			MapManager::getMe().getMap()->addBigImage(image);

			target->setVisible(false);
			if (target->isModel())
			{
				target->getWindow()->popModel();
			}
		}
	}
	CreateBigImageLogic(UIWindow *window):window(window){}
	UIWindow *window;
};
/**
 * ��������ϵ��///////////////////////////////////////////////////////////////////
 **/
class stFrameItem:public BaseUIItem<stFrameItem>
{
public:
	std::string name;
	stFrameItem()
	{
	
	}
}; // ÿ��֡
class stAddImageForCartoon:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel *target = base->getPanel();
			std::string fileName = target->getUILablevalue("filename");
			UIPanel *dirFrameDialog = window->getPanel("cartoonedit/createcartoon");
			if (dirFrameDialog)
			{
				// �����Ŀ��list
				UISuperBag *list = LIST(dirFrameDialog,"list");
				if (list)
				{
					stFrameItem *fileItem = stFrameItem::create("cartoonedit/frame_item.xml");
					fileItem->name = fileName ;
					fileItem->getPanel()->setUILabelvalue("bgname",fileItem->name);
					list->addItem(fileItem);
					CCSprite * content = CCSprite::create(fileItem->name.c_str());
					if (content)
					{
						content->setAnchorPoint(ccp(0,0));
						fileItem->addChild(content);
					}
				}
				list->show();
			}
		}
	}
	stAddImageForCartoon(UIWindow *window):window(window){}
	UIWindow *window;
}; // ����һ��ͼƬ

struct stTakeEachImage:stBagExecEach{
	stTakeEachImage(CartoonInfo *cartoonInfo):cartoonInfo(cartoonInfo)
	{}
	CartoonInfo *cartoonInfo;
	std::vector<std::string> frames;
	void exec(UIItem *item)
	{
		stFrameItem *frameItem = (stFrameItem*) item;
		if (frameItem)
		{
			CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(frameItem->name.c_str());
			CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(texture,CCRectMake(0,0,texture->getContentSize().width,texture->getContentSize().height));
			frame->retain();
			cartoonInfo->frames.push_back(frame);
			frames.push_back(frameItem->name.c_str());
		}
	}
};
class stTestCartoon:public UICallback{
public:
	void callback(UIBase *base)
	{
		// ��ȡlist�е�ÿ��Ԫ�� �������� �� ������Panel ����
		UIPanel *dirFrameDialog = window->getPanel("cartoonedit/createcartoon");
		if (dirFrameDialog)
		{
			// �����Ŀ��list
			CartoonInfo cartoonInfo;
			UISuperBag *list = LIST(dirFrameDialog,"list");
			MutiCartoon *cartoon = MutiCartoon::create();
			std::string cartoonName = dirFrameDialog->getEditFieldValue("cartoonname");
			if (cartoonName == "") return;
			if (list)
			{
				stTakeEachImage take(&cartoonInfo);
				list->execEachItem(&take);
				FileSet fileSet;
				for (int index = 0; index < take.frames.size();index++)
				{
					fileSet.addFile(take.frames[index]);
					cartoon->pngNames.push_back(take.frames[index]);
				}	
				fileSet.save(cartoonName.c_str(),".\\"); // ����һ�������Ķ���������
			}
			cartoonInfo.cartoonType = CartoonInfo::SELF;
			cartoonInfo.frameType = CartoonInfo::TIME_FRAMES;
			cartoonInfo.needTime = 1;
			GET_UI_BYNAME(dirFrameDialog,UIChoice,choice,"locationframe");
			if (choice && choice->isChoiced())
			{
				cartoonInfo.frameType = CartoonInfo::MOVE_FRAMES;
			}
			CCAnimate *animate = CCAnimate::create(cartoonInfo.createAnimation());
			
			cartoon->tapTime = 1;
			cartoon->cartoonName = cartoonName.c_str();
			cartoon->rebuild();
			
			MapManager::getMe().getMap()->addCartoon(cartoon);

			cartoonInfo.release();
		
		}
	}
	stTestCartoon(UIWindow *window,CCNode *edit):window(window),edit(edit){}
	UIWindow *window;
	CCNode *edit;
}; // ����

///////END//����///////////////////////////////////////////////////////////////////////////////

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
};

// �����ͼ////////////////////////////

class SaveMapLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			std::string value = panel->getEditFieldValue("filename");
			MapManager::getMe().mapRoot->write(value);
			
			panel->setVisible(false); // ����������
			if (panel->isModel())
			{
				panel->getWindow()->popModel();
			}
		}
	}
};

////�����赲����Ϣ////////////////////////

struct SetBlockInfoLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			int  value = 0; 
			panel->getEditFieldValue("blockvalue",value);
			{
				MapManager::getMe().isSetBlock = true;
				MapManager::getMe().blockValue = value;
			}
		}
	}
};
struct ClearBlockInfoLogic:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *panel = base->getPanel();
		if (panel)
		{
			MapManager::getMe().isSetBlock = false;
			MapManager::getMe().blockValue = 0;
			panel->hide();
		}
	}
};

/// չʾ��ͼ�߼�
/**
 * չʾ��������չʾ���Խ���/////////////////////////////////////
 */

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
/**
 * ���������ϵ�ÿ����ͼ
 */
struct stShowEachBg:public stExecEachBackgroud{
public:
	void exec(MutiMap *map)
	{
		MapInfoItem *item = MapInfoItem::create("maprop/iteminfo.xml");
		if (item)
		{
			UIPanel *targetPanel = window->getPanel("maprop/showmap");
			if (targetPanel)
			{
				// �ȱ����Ƿ������ͬ��
				UISuperBag * list = LIST(targetPanel,"list");
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
				panel->getEditFieldValue("x",map->m_tPosition.x);
				panel->getEditFieldValue("y",map->m_tPosition.y);
				map->setZOrder(atoi(panel->getEditFieldValue("zorder").c_str())); // ����zorder ����
				std::string name = panel->getEditFieldValue("name");
				if (name != "")
					map->fileName = name; 
			}
		}
	}
};

struct stShowMapProp:public UICallback{
public:
	void callback(UIBase *base)
	{
		if (window)
		{
			UIPanel *panel = window->showPanel("maprop/showmap");// ��showmap.xml ��Panel
			if (panel)
			{
				stShowEachBg exec;
				exec.window = window;
				MapManager::getMe().mapRoot->execEachBg(&exec); // ˢ���б�
				panel->setVisible(true);
			}
		}
	}
	UIWindow *window;

	stShowMapProp(UIWindow *window):window(window)
	{}
};
/**
 * �����µı���
 **/
struct stAddLayer:public UICallback{
public:
	void callback(UIBase *base)
	{
		MapInfoItem *item = MapInfoItem::create("maprop/iteminfo.xml");
		if (item)
		{
			UIPanel *targetPanel = window->getPanel("maprop/showmap");
			UIPanel *infoPanel = window->getPanel("maprop/addnewlayer");
			if (targetPanel && infoPanel)
			{
				std::string newLayerName = EIDTFIELD(infoPanel,"bgname")->getContent().c_str();
				if (newLayerName == "") return;
				LIST(targetPanel,"list")->addItem(item);
				
				LIST(targetPanel,"list")->show();
				
				// ���������ĵ�ͼ������
				GET_UI_BYNAME(item->getPanel(),UILabel,bgName,"bgname");
				if (bgName)
				{
					bgName->setContent(newLayerName.c_str()); // ��������
				}
				// ����һ����ͼ
				MutiMap *muMap = MutiMap::create("maprop/defaultbg.xml");
				
				if (muMap && MapManager::getMe().getMap())
				{
					muMap->fileName = newLayerName;
					MapManager::getMe().getMap()->addMap(muMap);
					MapManager::getMe().choiceMap(muMap); // ��ǰ��ͼΪ�༭����
				}
				item->getPanel()->bindBtnClick("edit",new stEditItem(item,muMap)); // ���¼�
				item->getPanel()->bindChoiceClick("show",new stShowItem(item,muMap)); // ���¼�
				item->mapName = bgName->getContent();
			}
		}
	}
	stAddLayer(UIWindow *window):window(window)
	{
	
	}
	UIWindow *window;
};
	

// ���ӹ���//////////////////////
struct stAddMonster:public UICallback{
public:
	void callback(UIBase *base)
	{
		UIPanel *targetPanel = window->getPanel("createmonster/createmonster");
		if (targetPanel)
		{
			std::string monstername= targetPanel->getEditFieldValue("monstername");
			if (monstername == "")
			{
				return;
			}
			MutiMonster *monster = MutiMonster::create();
			if (monster)
			{
				targetPanel->getEditFieldValue("bossai",monster->monsterAIID);
				monster->start(monstername.c_str());
				MapManager::getMe().mapRoot->addMonster(monster);
				monster->setPosition(GridIndex(0,2));
			}
		}
	}
	stAddMonster(UIWindow *window):window(window)
	{
	
	}
	UIWindow *window;
};

// ��ʼ���¼���

class LogicCallback:public UIEventCallback{
public:
	void init(UIWindow *window)
	{
		UICallbackManager::getMe().addCallback("do_add_big_image",new CreateBigImageLogic(window));
		UICallbackManager::getMe().addCallback("do_add_image",new CreateImageLogic());

		UICallbackManager::getMe().addCallback("do_add_image_for_cartoon",new stAddImageForCartoon(window));
		UICallbackManager::getMe().addCallback("testcatoon",new stTestCartoon(window,window)); // �󶨰�ť����Ӧ�¼�

		UICallbackManager::getMe().addCallback("openmaplogic",new OpenMapLogic()); // �󶨰�ť����Ӧ�¼�

		UICallbackManager::getMe().addCallback("savemaplogic",new SaveMapLogic()); // �󶨰�ť����Ӧ�¼�

		UICallbackManager::getMe().addCallback("set_map_block",new SetBlockInfoLogic()); // �󶨰�ť����Ӧ�¼�

		UICallbackManager::getMe().addCallback("cancel_set_map_block",new ClearBlockInfoLogic()); // �󶨰�ť����Ӧ�¼�
	
		UICallbackManager::getMe().addCallback("do_show_map_prop",new stShowMapProp(window)); // չʾ��ͼ����

		UICallbackManager::getMe().addCallback("add_new_layer",new stAddLayer(window)); // �����µĲ�
		
		UICallbackManager::getMe().addCallback("do_add_monster",new stAddMonster(window)); // �����µĲ�
	}
};
NS_CC_END