#include "Cartoon2DEdit.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIXmlBag.h"
#include "MapManager.h"
#include "MutiCartoon.h"
#include "MutiImage.h"
#include "MutiMonster.h"
#include "UILib.h"
#include "Cartoon.h"
#include "UIFileList.h"
#include "FileSet.h"
NS_CC_BEGIN
class stFrameItem:public BaseUIItem<stFrameItem>
{
public:
	std::string name;
	stFrameItem()
	{
	
	}
}; // ÿ��֡
class stAddImage:public UICallback{
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
	stAddImage(UIWindow *window):window(window){}
	UIWindow *window;
}; // ����һ��ͼƬ

struct stTakeEachImage:stBagExecEach{
	stTakeEachImage(Cartoon2DEdit *edit,CartoonInfo *cartoonInfo):edit(edit),cartoonInfo(cartoonInfo)
	{}
	Cartoon2DEdit *edit;
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
			if (list)
			{
				stTakeEachImage take(edit,&cartoonInfo);
				list->execEachItem(&take);
				FileSet fileSet;
				for (int index = 0; index < take.frames.size();index++)
				{
					fileSet.addFile(take.frames[index]);
				}	
				fileSet.save("temp.cartoon",".\\"); // ����һ�������Ķ���������
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

			edit->cartoon->runAction(CCRepeatForever::create(animate));

			cartoonInfo.release();
		
		}
	}
	stTestCartoon(UIWindow *window,Cartoon2DEdit *edit):window(window),edit(edit){}
	UIWindow *window;
	Cartoon2DEdit *edit;
}; // ����

void Cartoon2DEdit::doInitEvent()
{
	// ���ڲ��߼���ϵͳ
	UICallbackManager::getMe().addCallback("do_add_image",new stAddImage(window)); // �󶨰�ť����Ӧ�¼�
	UICallbackManager::getMe().addCallback("testcatoon",new stTestCartoon(window,this)); // �󶨰�ť����Ӧ�¼�

	cartoon = CCSprite::create();
	if (cartoon)
	{
		this->addChild(cartoon);
	}
}

NS_CC_END