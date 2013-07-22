#pragma once
#include "cocos2d.h"
#include "xmlScript.h"
#include "HexagonGrids.h"
#include "MutiObject.h"
NS_CC_BEGIN
class MutiMap;
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
		REPLACE_ACTION = 3, // �滻ִ��
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
	enum MOVESTATE{ // ��ʾ��ǰһ����״̬ �����ǰһ����
		NULL_STATE = -1,
		UPING = 0, // ����
		DOWNING = 1, // �½�
		LEFTING = 2, // ����
		RIGHTING = 3, // ����
	};
	MOVESTATE moveState;
	MoveActionPoint()
	{
		moveState = NULL_STATE;
	}
	void takeNode(script::tixmlCodeNode *node);
};
/**
 * Ԥ�����·��˵��
 */
class PrePathDec{
public:
	int dir; // ����
	std::vector<MoveActionPoint> indexs; // �����б�
	void takeNode(script::tixmlCodeNode *node);
};
/**
 * ���������·��
 **/
class PrePathDecDirs{
public:
	std::string name; // ����
	std::vector<PrePathDec> paths;
	void takeNode(script::tixmlCodeNode *node);
};
/**
 * ʵ�ֹ����ڵ�ͼ�ϵĸ��ֶ���
 */
class MutiMonster:public MutiObject,public script::tixmlCode{
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

	static std::map<std::string,PrePathDecDirs> prePaths; // ����Ԥ�����·��
	typedef std::map<std::string,PrePathDecDirs>::iterator PRE_PATHS_ITER;

	bool checkIn(const CCPoint &point);
	
	std::vector<GridIndex> myindexs; // �Լ��ĸ����б� �������м���
	std::vector<GridIndex> leftindexs; // �������
	std::vector<GridIndex> botoomindexs; // �±�����
	std::vector<GridIndex> rightindexs; // �ұ�����
	std::vector<GridIndex> upindexs; // �ϱ�����
	GridIndex getStartMyIndex(); // ��ȡ�Լ�����ʼ����
	/**
	 * ��鵱ǰһ������ �Ƿ���ײ
	 */
	bool checkCollideInMap(const GridIndex& nextIndex);
	/**
	 * ���ֻ�ǵײ���ײ��
	 */
	bool checkOnlyButtomCollide(const GridIndex &nextIndex);

	/**
	 * �²�û����ײ
	 */
	bool checkNotDownCollide(const GridIndex &nextIndex);
	/**
	 * ��û����ײ
	 */
	bool checkNotLeftCollide(const GridIndex &nextIndex);
	/**
	 * �ϲ�û����ײ
	 */
	bool checkNotUpCollide(const GridIndex &nextIndex);
	/**
	 * �Ҳ�û����ײ
	 */
	bool checkNotRightCollide(const GridIndex &nextIndex);

	bool checkNotDirCollide(const GridIndex &nextIndex,std::vector<GridIndex> &dirindexs,int index=1);
	/**
	 * ��ȡ��Ҫ�½��ĵ������
	 */
	GridIndex getPreDownIndex();
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
	/**
	 * ʹ��Star Ѱ·��Ŀ�ĵ�
	 */
	void tryMoveUseAstr(const GridIndex &index);
	/**
	 * ���Astar Ѱ·
 	 */
	void clearAstar();
	void setPosition(const GridIndex &point);
	/**
	 * ��ȡ��point Ϊ��ʼ��·���б� ������ǰ·��
	 */
	void getPreparePath(const std::string &name,const GridIndex &point);

	void setPosition(const CCPoint &point);

	void freshBlock();
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
	void jumpTo();

	void moveLeft();
public:

};

NS_CC_END