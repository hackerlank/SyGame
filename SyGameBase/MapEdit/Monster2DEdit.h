/**
 * ���ﶯ���༭��
 * ����һ�鶯�� ���浽�ļ� ���� 
 * 1.����1 ������
 * 2.����2 ������
 * 3.����3 ���ŵ�ָ���� ��������ײ ���Դ���ص��߼�����
 * 4.����4 ����ط����� ��������ײ ���Դ���ص��߼�����
 */
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
class Monster2DEdit:public BaseDialog<Monster2DEdit>{
public:
	virtual void doInitEvent();
};

NS_CC_END