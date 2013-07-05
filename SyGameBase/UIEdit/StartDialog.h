#pragma once
/**
 * ��Ϸ������
 */
#include "cocos2d.h"
#include "UIPanel.h"
#include "UIItem.h"
#include "UIChoiceList.h"
#include "UIBag.h"
#include "UILabel.h"
#include "UIEditField.h"
#include "UIWindow.h"
NS_CC_BEGIN

class StartDialog:public BaseDialog<StartDialog>{
public:
	virtual void doInitEvent();
};

class DefaultDialog:public BaseDialog<DefaultDialog>{
public:
};

/**
 * �������Button �� sysui
 */
class MainDialog:public BaseDialog<MainDialog>{
public:
	virtual void doInitEvent();
};

/**
 * ����Panel 
 */
class PropDialog :public BaseDialog<PropDialog>
{
public:
	virtual void doInitEvent();
};

/**
 * һ��Label 
 * һ���ı���
 * ������ť[Sure] [Cancel]
 */
class SavePanelDialog:public BaseDialog<SavePanelDialog>
{
public:
	virtual void doInitEvent();
	void savePanel(const std::string &name);
	UIPanel *nowPanel;
	SavePanelDialog()
	{
		nowPanel = NULL;
	}
};
NS_CC_END