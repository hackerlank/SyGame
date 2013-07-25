#pragma once 

/**
 * ����AI
 */

#include "scriptLib.h"
#include "cocos2d.h"
NS_CC_BEGIN
class MutiMonster;
class MutiAIStub;
class MonsterAILib;
class MutiAI{
public:
	MutiAI()
	{
		id = 0;
	}
	DWORD id;
	/**
 	 * ִ��һ���¼�
 	 * */
	bool action(MutiAIStub * stub,int event);
	enum{
		DEATH = 0, // ����
		BIRTH = 1, // ����
		TARGET_ENTER = 2, // ���������Ұ
		TARGET_LEAVE = 3, // �����뿪��Ұ
		ATTACK_ME = 4, // ���˹���
		IDLE_ACTION = 5, // ����ʱ���
		MEET_TARGET = 6, // �����ڹ�����Χ�� 
		HAD_TARGET = 7, // ��Ŀ���״̬
	};
	/**
 	 * ����һ��code
 	 * \param code ����ڵ�
 	 * \param name ��������
 	 * \return ture �ɹ� false ʧ��
 	 * */
	bool addCode(script::tixmlCodeNode* code,std::string name);
private:
	std::vector<script::tixmlCodeNode*> events;	
};
typedef int (MonsterAILib::*AIAction)(MutiAIStub *,script::tixmlCodeNode *);

//ִ�е�
class AILibAction{
public:
	int type;
	AIAction action;
};
class MutiMonsterRefrence
{
public:
	MutiMonster * monster;
	DWORD uniqueId;

	bool isValid(){return true;} // ��ǰ�����Ƿ���Ч
};
/**
 * ai ��ִ����
 * */
class MutiAIStub{
public:
	MutiMonster *npc;
	std::vector<MutiMonsterRefrence> targetPool; // ����� 0 �Ŷ���ΪĬ�ϴ����
	typedef std::vector<MutiMonsterRefrence>::iterator TARGETPOOL_ITER;
	MutiAIStub()
	{
		npc = NULL;
	}
	MutiAIStub(MutiMonster *npc)
	{
		this->npc = npc;
	}
};
/**
 * ִ�п�
 * ���ش���� �� �¼���
 **/
class MonsterAILib:public script::Lib<AILibAction,MutiAIStub,MonsterAILib>
{
public:
	static MonsterAILib & getMe()
	{
		static MonsterAILib lib;
		return lib;
	}
	void initWithFile(const char *fileName);
    /**
     * ֻ����scope="stub" ��ʱ�� type ����Ч
     */
    int var(MutiAIStub* stub,script::tixmlCodeNode * node)
    {
        return script::Lib<AILibAction,MutiAIStub,MonsterAILib>::var(stub,node);
    }
    /**
     * ���
     */
    int print(MutiAIStub* stub,script::tixmlCodeNode * node)
    {
    	return script::Lib<AILibAction,MutiAIStub,MonsterAILib>::print(stub,node);
    }
    /**
     * �������
     **/
    int calc(MutiAIStub* stub,script::tixmlCodeNode * node)
    {
    	return script::Lib<AILibAction,MutiAIStub,MonsterAILib>::calc(stub,node);
    }
    /**
     * ִ�������ڵ�
     **/
    int exec(MutiAIStub* stub,script::tixmlCodeNode * node)
    {
        execCode(stub,node->getAttr("name"));
        return 0;
    }
    void bindActions();
    void takeNode(script::tixmlCodeNode *node);
    /**
     * �����Դ
     * */
    void destroy();
    /**
     * ִ���¼�
     * \param npcAIID ai ��
     * \param npc ����
     * \param event �¼�
     * */
    void execEvent(DWORD npcAIID,MutiAIStub *stub,int event);
    std::vector<MutiAI*> npcAis;
/////////////////////////////////////////////////////////////////////////    
//			�����չ
////////////////////////////////////////////////////////////////////////
    /**
     * ��������
     * */    
    int putskill(MutiAIStub* stub,script::tixmlCodeNode * node);

	/**
	 * ���ߵ� Ŀ��
	 * <movetotarget/>
	 **/
	
	/**
	 * �趨������̵�Ϊ��ǰ�������� �����趨lock����
	 * <lockmindistacetarget/>
	 */
	
	/**
	 * ��Ȩֵ��������� �����趨
	 * <locksuittarget/>
	 **/

	/**
	 * ���ߵ�Ŀ�ĵ�
	 * <move targetx="" targety=""/>
	 */
	
	/**
	 * ����Ŀ����Ȧ
	 * <moverandarround/>
	 */
};

#define theAILib MonsterAILib::getMe()

NS_CC_END
