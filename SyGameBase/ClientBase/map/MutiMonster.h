#pragma once
#include "cocos2d.h"
#include "xmlScript.h"
#include "Cartoon.h"
#include "MutiMap.h"
NS_CC_BEGIN

class ActionPool{
public:
	static std::map<std::string,unsigned int> actionsMap;
	typedef std::map<std::string,unsigned int>::iterator ACTIONSMAP_ITER;
	unsigned int actions; // ����IDs
	std::list<unsigned int> actionStack;
	typedef std::list<unsigned int>::iterator ACTIONS_ITER;
	void addAction(const std::string &name);
	void addAction(const unsigned int &actionId);
	enum ACTION_TYPE{
		CONBINE_ACTION = 0, // ͬʱ��Ϊ
		SINGLE_ACTION = 1, // ������Ϊ
		ALWAYS_ACTION = 2, // ������Ϊ ���뿪
	};
	ACTION_TYPE actionType; // ��Ϊ����
	/**
	 * ��ȡ����
	 */
	unsigned int getAction();
	/**
	 * �ͷŹ���
	 */
	void popAction();

	static unsigned int getActionIdByName(const std::string&name); 

	ActionPool()
	{
		actions = 0;
		actionType = SINGLE_ACTION;
	}
};

class MoveActionPoint{
public:
	GridIndex index; //��ǰ�ƶ���λ��
	std::string actionName; // ��ǰ�ƶ�����Ϊ
};
/**
 * Ԥ�����·��˵��
 */
class PrePathDec{
public:
	int dir; // ����
	std::vector<MoveActionPoint> indexs; // �����б�
};
/**
 * ���������·��
 **/
class PrePathDecDirs{
public:
	std::string name; // ����
	std::vector<PrePathDec> paths;
};
/**
 * ʵ�ֹ����ڵ�ͼ�ϵĸ��ֶ���
 */
class MutiMonster:public Cartoon,public script::tixmlCode{
public:
	static MutiMonster * create();
	int dir;
	CCPoint offset; // ���ƫ��
	CCSize size; // �ҵ���Ч���
	MutiMap *map;
	MutiMonster()
	{
		dir = 0;
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
	void initNode(script::tixmlCodeNode *node);
	std::vector<ActionPool> actionPools;
	typedef std::vector<ActionPool>::iterator ACTIONPOOLS_ITER;
	std::map<std::string,int> priorityMap; // ���ȼ��б� �������м���
	typedef std::map<std::string,int>::iterator PRIORITYMAP_ITER;

	CartoonInfo * makeMyCartoon(const unsigned int&,int dir);
	void putMyCartoon(CartoonInfo *info);
	typedef std::vector<CartoonConbineAction> COBINE_ACTIONS;
	static std::map<unsigned int,COBINE_ACTIONS > conbineactionmaps; // ��Ϊ��
	typedef std::map<unsigned int,COBINE_ACTIONS >::iterator CONBINEACTIONMAPS_ITER;
	unsigned int nowActionName;

	static std::map<std::string,PrePathDecDirs> PRE_PATHS; // ����Ԥ�����·��
	typedef std::map<std::string,PrePathDecDirs>::iterator PRE_PATHS_ITER;

	
	std::vector<GridIndex> myindexs; // �Լ��ĸ����б� �������м���

	GridIndex getStartMyIndex(); // ��ȡ�Լ�����ʼ����
	/**
	 * ��鵱ǰһ������ �Ƿ���ײ
	 */
	bool checkCollideInMap(const GridIndex& nextIndex);
	/**
	 * ���ݵ��ȡ��ͼ��ʵ������λ��
	 */
	CCPoint getLocationByIndex(const GridIndex &index);
	/**
	 * ��ȡ��ǰ������
	 */
	GridIndex getNowIndex();

	/**
	 * ��鵱ǰ��Ϊ���Ƿ�����Լ�
	 */
	bool isNowAction(const std::string &name);

	void tryMove(const GridIndex &index);

	void setPosition(const GridIndex &point);
public:
	/**
	 * �жϵ�ǰ�Ƿ��ƶ�
	 */
	bool isMoving();
	
	
	/**
	 * ��ǰĿ�ĵ� ʹ����Astar
	 */
	MoveActionPoint nowAstarDestination;
	/**
	 * ���ƶ���·��
	 */
	std::list<MoveActionPoint> movePath;
	/**
	 * ��ǰĿ�꼯��
	 */
	std::list<MutiMonster*> targets;
	/**
	 * ��ȡ��ǰ��Ŀ��
	 */
	MutiMonster * getNowTarget();
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