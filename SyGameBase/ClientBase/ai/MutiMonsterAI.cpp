#include "MutiMonsterAI.h"
#include "MutiMonster.h"
NS_CC_BEGIN
bool MutiAI::action(MutiAIStub *stub,int event)
{
	if (event >= events.size()) return false;
	script::tixmlCodeNode *code = events.at(event);
	if (!code) return false;
	return theAILib.execCode(stub,code);
}
#define BIND_AI_EVENT(event)\
	if (name == std::string(#event))\
	{\
		if (event >= events.size())\
		{\
			events.resize(event + 1);\
		}\
		events[event] = code;\
	}
bool MutiAI::addCode(script::tixmlCodeNode *code,std::string name)
{
	if (name == std::string("DEATH"))
	{
		if (DEATH >= events.size())
		{
			events.resize(DEATH + 1);
		} 
		events[DEATH] = code;
	}
	else if (name == std::string("BIRTH"))
	{
		if (BIRTH >= events.size())
		{
			events.resize(BIRTH + 1);
		}
		events[BIRTH] = code;
	}
	
	// �Ժ���չ��ӭʹ�ú�
	BIND_AI_EVENT(TARGET_ENTER); // ���������Ұ	
	BIND_AI_EVENT(TARGET_LEAVE); // �����뿪��Ұ
	BIND_AI_EVENT(IDLE_ACTION); // ������Ϊ
	BIND_AI_EVENT(ATTACK_ME); // ��������
	BIND_AI_EVENT(MEET_TARGET); // Ŀ���ڹ�����Χ��
	BIND_AI_EVENT(HAD_TARGET_LEAVE); // �ж����뿪
	BIND_AI_EVENT(HAD_TARGET); //�ж���
	return true;
}

int MutiAIStub::getTargetCount() // ��ǰ���������
{
	return targetPool.size();
}
MutiMonster * MutiAIStub::getTarget() // ��ȡ��ǰ����
{
	if (targetPool.size()) return targetPool.at(0).monster;
	return NULL;
}
void MutiAIStub::addTarget(MutiMonster *monster) // ���Ӷ���
{
	
}
#define BIND_AI_ACTION(func)\
	{\
		AILibAction *action = new AILibAction;\
		action->type = script::__ACTION_CHECK__;\
		action->action = &MonsterAILib::func;\
		actions[#func] = action;\
	}

void MonsterAILib::bindActions()
{
	BIND_AI_ACTION(var);	
	BIND_AI_ACTION(print);
	BIND_AI_ACTION(calc);
	BIND_AI_ACTION(exec);
// ���npc ai ����չ
	BIND_AI_ACTION(putskill);
}
void MonsterAILib::initWithFile(const char *fileName)
{
	bindActions();
	std::string startui = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName);
	unsigned long nSize = 0;
	unsigned char * buffer = CCFileUtils::sharedFileUtils()->getFileData(startui.c_str(),"rb",&nSize);
	if (!nSize)return;
	initFromString((char*)buffer);
}
void MonsterAILib::takeNode(script::tixmlCodeNode* node)
{
	if (!node) return; // �����¼�
	script::tixmlCodeNode aiNode = node->getFirstChildNode("ai");
	while (aiNode.isValid())
	{
		script::tixmlCodeNode codePtr = aiNode.getFirstChildNode("event");
		MutiAI *ai = new MutiAI();
		aiNode.getAttr("id",ai->id);
		if (ai->id >= npcAis.size())
		{
			npcAis.resize(ai->id + 1);
		}
		if (!npcAis[ai->id])
		{
			npcAis[ai->id] = ai;	
		}
		else
			delete ai;
		while (codePtr.isValid())
		{
			std::string name = codePtr.getAttr("name");
			std::string code = codePtr.getAttr("code");
			npcAis[ai->id]->addCode(findCode(code.c_str()),name);
			codePtr = codePtr.getNextNode("event");	
		}	
		aiNode = aiNode.getNextNode("ai");
	}		
}
void MonsterAILib::destroy()
{
	for (std::vector<MutiAI*>::iterator iter = npcAis.begin();
		 iter != npcAis.end();++iter)
	{
		MutiAI * npcAi = *iter;
		if (npcAi)
		{
			delete npcAi;
		}
	}
	npcAis.clear();
	script::Lib<AILibAction,MutiAIStub,MonsterAILib>::release();
}
void MonsterAILib::execEvent(DWORD npcAIID,MutiAIStub *stub,int event)
{
	if (npcAIID < npcAis.size())
	{
		MutiAI * ai = npcAis.at(npcAIID);
		if (ai)
		{
			ai->action(stub,event);	
		}
	}	
}
int MonsterAILib::putskill(MutiAIStub* stub,script::tixmlCodeNode * node)
{
	if (stub->npc)
	{
		stub->npc->tryAction(node->getAttr("name"));
	}
	return 1;
}
NS_CC_END