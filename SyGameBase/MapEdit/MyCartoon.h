#include "cocos2d.h"

USING_NS_CC;

/**
 * ֡����
 */
class MyAnimation{
public:
	std::vector<CCSpriteFrame*> frames;

	CCSpriteFrame * getFrame(int index);

	void destroyFrames();
};
/**
 * ���� �������������� ʹģ��֧�ֻ�װ ����һ���༭�� ʹ��lua��д
 */
class MyCartoon:public CCNode{
public:
	std::vector<CCSprite*> sprites; // ���鼯��
	std::vector<MyAnimation*> animations; // ��������
	/**
	 * actionId ��ǰ����Ϊ���
	 * չʾ ��startFrame �͵� endFrame ��ʱʱ�� timeout
	 * blockId ��ָ�ڼ���
	 */
	void show(int actionId,int startFrame,int endFrame,int timeout,int blockId);

	void setFrame(int id,int blockId); // ���õ�ǰ֡

	void end(int actionId,int blockId); // ��ǰһ����Ϊ������
};