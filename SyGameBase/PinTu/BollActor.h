#pragma once
#include "Cartoon.h"

NS_CC_BEGIN
/**
 * ��Ա������
 */
struct stBollActorData{
	/**
	���ߣ����塢����
	Ͷ�������֡���Ͷ
	���أ����š���ñ
	���ʣ��ٶȡ�����
	 */
	float lanban;
	float power;
	float sanfen;
	float zhongtou;
	float ganrao;
	float gaimao;
	float speed;
	float tineng;
};
/**
 * ��Ա �̳���Cartoon ���Խ��ж�����Ϊ
 */
class BollActor:public Cartoon{
public:
	enum{
		ACTION_IDLE, // ����
		ACTION_MOVE, // �ƶ�
		ACTION_TOULAN, // Ͷ��
		ACTION_FANGSHOU, // ����
		ACTION_KOULAN, // ��������
		ACTION_TALK, // ˵��
	};
	stBollActorData data;
	BollActor()
	{
		
	}
	bool init();
	static BollActor *create();
	/**
	 * ����Ƿ�����Ϊ������ ʧ����Ϊ���� 
	 */
	bool checkInPercent(int value);
	/**
	 * �������
	 */
	int calcDistance(BollActor *actor);

	/**
	 * ��������һ��
	 */
	virtual void nextStep();
	virtual CartoonInfo* v_makeCartoon(int actionType);
	virtual void v_putCartoon(CartoonInfo *action);
	/**
	 * ��ȡ������
	 */
	CCPoint getDefendPoint(BollActor *defender);

	/**
	 * ��ȡ��һ�����ƶ���
	 */
	CCPoint getCanWalkPoint();
};
class ActorAction{
public:
	int id;
	int actionType;
	std::vector<CCPoint> destinations;
	std::string cartoonName;
	CCPoint location;
	enum{
		WALK,
		IDLE,
	};
	/**
	 * ִ��
	 */
	void actor(BollActor *actor);
	
	CCPoint getRandomPoint();

	void takeNode(script::tixmlCodeNode *node);

	ActorAction &operator = (const 	ActorAction actor)
	{
		id = actor.id;
		actionType = actor.actionType;
		destinations = actor.destinations;
		cartoonName = actor.cartoonName;
		return *this;
	}
};
class BollActorManager;
/**
 * �򳡶��� �򵥹̶�ʽ�趨
 * ����ÿ����ÿ����Ա��λ�� ����
 */
class BollStep{
public:
	std::string stepName;
	int lastTime; // ����ʱ��
	int actionsStepType; // ��ǰ��Ϊ����
	cc_timeval lastActionTime; // �ϴ�ִ��ʱ��
	int tickCount; // ��ʱ����
	enum{
		ACTIONS_STEP_FROM_XML = 0, // ��Ϊ�������� ʱ�������
		ACTIONS_STEP_AI = 1,
	};
	BollStep()
	{
		CCTime::gettimeofdayCocos2d(&lastActionTime,NULL);
		tickCount = -1;
	}
	bool checkTimeOut();
	void start(BollActorManager *bam);
	void takeNode(script::tixmlCodeNode *node);
	std::list<ActorAction> actorActions;
	typedef std::list<ActorAction>::iterator ACTORACTIONS_ITER;
};
/**
 * �����ָ����Ա
 */
class BollActorManager:public script::tixmlCode{
public:
	static BollActorManager&getMe()
	{
		static BollActorManager bam;
		return bam;
	}
	BollActorManager()
	{
		nowBollActor = NULL;
		nowScene = NULL;
	}
	void init();
	/**
	 * ��Ա�������� ���ж���
	 * 1. �ط�ռλ
	 * 2. ��������
	 * 3. ���ʶ���
	 * 4. ���� Ӳ��
	 * 5. ��Ա����
	 * 6. ����
	 * 7. ��������
	 */
	void actor(BollActor * actor);
	/**
	 * �峡
	 */
	void clear();
	
	/**
	 * ͨ��actionType��ȡ������Ϣ
	 */
	CartoonInfo *getCartoonByActionType(int actionType);
	CartoonInfo *getCartoonByActionName(const std::string& actionName,int dir);
	bool checkBollInMineTeam(BollActor *actor);
	/**
	 * �̶���������
	 */
	void tick();

	BollActor * getActorById(int id);

	CCNode *nowScene; // ��ǰ����

	void start(const std::string& name);
private:
	std::list<BollStep*> steps;

	std::list<BollStep*> tickSteps; // ÿʱÿ�� ���в���
	BollStep *nowStep; // ��ǰ�Ĳ���
	BollActor *nowBollActor; // ��ǰ�������
	std::vector<BollActor*> actors;
	typedef std::vector<BollActor*>::iterator BOLLACTORS_ITER;

	std::map<std::string,std::list<BollStep*> > namesteps;
	typedef std::map<std::string,std::list<BollStep*> >::iterator NAMESTEPS_ITER;
	/**
	 * ϵͳ�е����ȭ����
	 */
	typedef std::vector<CartoonConbineAction> COBINE_ACTIONS;
	std::map<std::string,COBINE_ACTIONS > conbineactionmaps;
	typedef std::map<std::string,COBINE_ACTIONS>::iterator CONBINEACTIONMAPS_ITER; 
	/**
	 * �������ļ��м�������
	 * \param node ���ø��ڵ�
	 */
	void takeNode(script::tixmlCodeNode *node);
};

#define theBollManager BollActorManager::getMe()
NS_CC_END