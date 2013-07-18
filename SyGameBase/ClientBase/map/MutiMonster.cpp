#include "MutiMonster.h"

NS_CC_BEGIN
static std::map<std::string,MutiMonster::COBINE_ACTIONS > conbineactionmaps;


MutiMonster * MutiMonster::create()
{
	MutiMonster *monster = new MutiMonster();
	if (monster)
	{
		monster->initWithTexture(NULL);
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return NULL;
}
/**
 * ���ԴӶ����б��л�ȡһ������ ����Ϊ
 * ���޶�����ִ����ʱ
 * �������ļ��л�ȡ�������ȼ��б� ����ʼ��
 * <actions count="10">
 *	<action name=""  priority=""/>
 * </actions>
 */
void MutiMonster::start(const std::string &actionFile)
{
	std::string startui = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(actionFile.c_str());
	unsigned long nSize = 0;
	unsigned char * buffer = CCFileUtils::sharedFileUtils()->getFileData(startui.c_str(),"rb",&nSize);
	if (!nSize)return;
	initFromString((char*)buffer);

	// �������ļ��м�����Ϣ
	doAction();
}
/**
 * ִ��ĳ������ ��ȡ��ǰ���������ȼ� Ȼ����뵽�б��� Ĭ��Ϊ0
 */
void MutiMonster::tryAction(const std::string& name)
{
	int step = 0;
	PRIORITYMAP_ITER iter = priorityMap.find(name);
	if (iter != priorityMap.end())
	{
		step = iter->second;
	}
	if (step >= actionPools.size())
	{
		actionPools.resize(step+1);
	}
	actionPools[step].actions.push_back(name);
}
/**
 * ��һ������ǰϦ ���ݶ���������Ϊ
 */
void MutiMonster::nextStep()
{
	/* TODO
	if (checkNowAction("move"))
	{
		// ����Ƿ����յ�
		// ���ڼ�����
		moveToUseAstar(nowDesitination);

		map->clearBlock(nowLocationIndex);
	}*/
	doAction();
}
/**
 * �������ļ��л�ȡ��Ϣ
 **/
void MutiMonster::takeNode(script::tixmlCodeNode *node)
{
	// ������϶���
	script::tixmlCodeNode conbinesNode = node->getFirstChildNode("cobineactions");
	if (conbinesNode.isValid() && conbineactionmaps.empty())
	{
		script::tixmlCodeNode actionsNode = conbinesNode.getFirstChildNode("actions");
		while(actionsNode.isValid())
		{
			std::string actionsName = actionsNode.getAttr("name");
			script::tixmlCodeNode actionNode = actionsNode.getFirstChildNode("action");
			COBINE_ACTIONS actions_vec;
			while(actionNode.isValid())
			{
				CartoonConbineAction rpgAction;
				rpgAction.takeNode(&actionNode);
				actions_vec.push_back(rpgAction);
				actionNode = actionNode.getNextNode("action");
			}
			conbineactionmaps[actionsName] = actions_vec;
			actionsNode = actionsNode.getNextNode("actions");
		}
	}
}

void MutiMonster::doAction()
{
	for (ACTIONPOOLS_ITER iter = actionPools.begin(); iter != actionPools.end();++iter)
	{
		if (iter->actions.size())
		{
			std::string name = iter->actions.front();
			CartoonInfo *cartoonInfo = makeMyCartoon(name,dir);
			if (cartoonInfo && cartoonInfo->isValid())
			{
				nowActionName = name;
				iter->actions.pop_front();
				putMyCartoon(cartoonInfo);
				return ;
			}
		}
	}
	// ִ��һ���ջص�
	CCSprite::runAction(CCSequence::create(CCDelayTime::create(0.5),
					CCCallFunc::create(this, callfunc_selector(MutiMonster::nextStep)),NULL));
	
}

CartoonInfo * MutiMonster::makeMyCartoon(const std::string &name,int dir)
{
	CONBINEACTIONMAPS_ITER iter = conbineactionmaps.find(name);
	if (iter != conbineactionmaps.end())
	{
		COBINE_ACTIONS& actions = iter->second;
		CartoonInfo *root = NULL;
		CartoonInfo * temp = NULL;
		for (COBINE_ACTIONS::iterator pos = actions.begin(); pos != actions.end();++pos)
		{
			if (temp)
			{
				temp->nextCartoon = pos->getCartoonInfo(dir);
				temp = temp->nextCartoon;
				temp->nextConbineType = pos->nextType;
			}
			else if (!root)
			{
				root = pos->getCartoonInfo(dir);
				temp = root;
				temp->nextConbineType = pos->nextType;
				continue;
			}
			else
			{
				break;
			}
		}
		return root;
	}
	return NULL;
}

void MutiMonster::putMyCartoon(CartoonInfo *info)
{
	/* ������
		if (info->cartoonName == "move")
		{
			CCPoint dest = getLocationByIndex(maybeLocationIndex);
			runAction(this->getParent(),info,dest,NULL);
		}
	*/
}

// �����ƶ���
void MutiMonster::moveToUseAstar(const GridIndex &point)
{
	/*
		ʹ��Astar �ƶ�
		GridIndex nextIndex;
		if (map->getNextPosition(getNowIndex(),point,nextIndex))
		{
			nowLocationIndex = getNowIndex();
			maybelLocationIndex = nextIndex;
			map->setBlock(maybeLocationIndex);
			map->setBlock(nowLocationIndex);
			nowDestination = point;
			tryAction("move");
		}
	*/
}
NS_CC_END