#pragma once
#include "cocos2d.h"
#include "xmlScript.h"
#include "Cartoon.h"
#include "MutiMap.h"
NS_CC_BEGIN

class ActionPool{
public:
	std::list<std::string> actions;
};
/**
 * ʵ�ֹ����ڵ�ͼ�ϵĸ��ֶ���
 */
class MutiMonster:public Cartoon,public script::tixmlCode{
public:
	static MutiMonster * create();
	int dir;
	MutiMap *map;
	MutiMonster()
	{
		dir = -1;
		map = NULL;
	}
	/**
	 * ���ԴӶ����б��л�ȡһ������ ����Ϊ
	 * ���޶�����ִ����ʱ
	 * �������ļ��л�ȡ�������ȼ��б� ����ʼ��
	 * <actions count="10">
	 *	<action name=""  priority=""/>
	 * </actions>
	 */
	void start(const std::string &actionFile);
	/**
	 * ִ��ĳ������ ��ȡ��ǰ���������ȼ� Ȼ����뵽�б��� Ĭ��Ϊ0
	 */
	void tryAction(const std::string& name);
	/**
	 * ��һ������ǰϦ ���ݶ���������Ϊ
	 */
	void nextStep();
	/**
	 * ִ����Ϊ
	 */
	void doAction();
	/**
	 * �������ļ��л�ȡ��Ϣ
	 **/
	void takeNode(script::tixmlCodeNode *node);
	std::vector<ActionPool> actionPools;
	typedef std::vector<ActionPool>::iterator ACTIONPOOLS_ITER;
	std::map<std::string,int> priorityMap; // ���ȼ��б�
	typedef std::map<std::string,int>::iterator PRIORITYMAP_ITER;

	CartoonInfo * makeMyCartoon(const std::string &name,int dir);
	void putMyCartoon(CartoonInfo *info);
	typedef std::vector<CartoonConbineAction> COBINE_ACTIONS;
	static std::map<std::string,COBINE_ACTIONS > conbineactionmaps;
	typedef std::map<std::string,COBINE_ACTIONS >::iterator CONBINEACTIONMAPS_ITER;
	std::string nowActionName;
public:
	/**
	 * �ƶ���ĳ��
	 */
	GridIndex destionation;
	GridIndex nowLocationIndex; // ���ڵĵ�
	GridIndex maybeLocationIndex; // �����ĵ�
	/**
	 * ��ǰ��Ŀ�ĵ�ʱ ����λ�� ���趨���ж��� ʹ��Astar �㷨 ��Astar�㷨ʧ�� ʹ���������Ч���ƶ��Ĳ��� ����Ҫ��ͷ 
	 * ���ȵ�ͷ �ȴ���һ������
	 */
	void moveToUseAstar(const GridIndex &point);
	/**
	 * ���Ե�ͷ
	 */
	void setDir(int dir);
	
	/**
	 * ����ʵ������µİ汾 ʹ������������ϵͳ ����·��Ԥ�ȶ���
	 * ���ռȶ����߼�·������ ���������׶���ײ ������(��ײ��ֹ) �����½��׶���ײ ��ֹ
	 */
	void jumpTo(const GridIndex &point);
public:

};

NS_CC_END