#pragma once

#include "MutiObject.h"
NS_CC_BEGIN
class MutiCartoon:public MutiObject{
public:
	/**
	 * �ӽڵ��ϴ�������
	 */
	static MutiCartoon * create(script::tixmlCodeNode *node);
	/**
	 * �������ļ��ж�ȡ��Ϣ
	 */
	virtual void readNode(script::tixmlCodeNode *node);
	bool init();
	/**
	 * ����Ϣд��ڵ㵱��
	 */
	TiXmlElement * writeNode(TiXmlElement *parent,const std::string &name);
private:
	std::vector<std::string> pngNames; // ͼƬ����
	float tapTime; // ���ʱ��
	int repeateTimes; // �ظ��Ĵ���
};
NS_CC_END