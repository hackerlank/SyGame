#include "MutiMonster.h"

NS_CC_BEGIN
std::map<unsigned int,MutiMonster::COBINE_ACTIONS > MutiMonster::conbineactionmaps;
std::map<std::string,unsigned int> ActionPool::actionsMap;
void ActionPool::addAction(const std::string &name)
{
	unsigned int id = getActionIdByName(name);
	if (id != 0)
	{
		addAction(id);
	}
}
void ActionPool::addAction(const unsigned int& actionId)
{
	if (actionType == ALWAYS_ACTION)
	{
		actions = actionId;
	}
	if (actionType == CONBINE_ACTION)
	{
		actions += actionId;
	}
	if (actionType == SINGLE_ACTION)
	{
		actionStack.push_back(actionId);
	}
}
/**
 * ��ȡ����
 */
unsigned int ActionPool::getAction()
{
	if (actionType == CONBINE_ACTION)
	{
		return actions;
	}
	if (actionType == SINGLE_ACTION)
	{
		if (actionStack.empty()) return 0;
		return actionStack.front();
	}
	if (actionType == ALWAYS_ACTION)
	{
		return actions;
	}
}
unsigned int ActionPool::getActionIdByName(const std::string&name)
{
	ACTIONSMAP_ITER iter = actionsMap.find(name);
	if (iter != actionsMap.end())
	{
		return iter->second;
	}
	return 0;
}
/**
 * �ͷŹ���
 */
void ActionPool::popAction()
{
	if (actionType == CONBINE_ACTION)
	{
		actions = 0;
	}
	if (actionType == SINGLE_ACTION)
	{
		actionStack.pop_front();
	}
}
MutiMonster * MutiMonster::create()
{
	MutiMonster *monster = new MutiMonster();
	if (monster)
	{
		monster->initWithTexture(NULL, CCRectZero);
		monster->autorelease();
		monster->attachSprite(monster);
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

	tryAction("idle");
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
	actionPools[step].addAction(name);
}
/**
 * ��һ������ǰϦ ���ݶ���������Ϊ
 */
void MutiMonster::nextStep()
{
	
//	 TODO
	map->clearBlock(nowLocationIndex);
//	��ǰ·����Ϊ�� ����Ŀ�ĵ���Ч
	if (isMoving())
	{
		tryAction(movePath.front().actionName);
	}
	if (nowAstarDestination.index.isValid() && nowAstarDestination.index.equal(getNowIndex()))
	{
		// TODO end
	}
	// ����Ƿ����յ�
	// ���ڼ�����
	if (nowAstarDestination.index.isValid() && !nowAstarDestination.index.equal(getNowIndex()) && !isMoving()) // ʹ��Astar ����Ѱ·
		moveToUseAstar(nowAstarDestination.index);
	doAction();
	
}
/**
 * �������ļ��л�ȡ��Ϣ
 **/
void MutiMonster::takeNode(script::tixmlCodeNode *node)
{
	initNode(node);
}
void MutiMonster::initNode(script::tixmlCodeNode *node)
{
	// ������϶���
	script::tixmlCodeNode conbinesNode = node->getFirstChildNode("conbineactions");
	if (conbinesNode.isValid() && conbineactionmaps.empty())
	{
		script::tixmlCodeNode actionsNode = conbinesNode.getFirstChildNode("actions");
		while(actionsNode.isValid())
		{
			unsigned int actionsName = actionsNode.getInt("name");
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
	script::tixmlCodeNode actionPriorityNode = node->getFirstChildNode("actionpriority");
	if (actionPriorityNode.isValid())
	{
		script::tixmlCodeNode priorityNode = actionPriorityNode.getFirstChildNode("priority");
		while (priorityNode.isValid())
		{
			std::string name = priorityNode.getAttr("name");
			int value = priorityNode.getInt("value");
			priorityMap[name] = value;
			priorityNode = priorityNode.getNextNode("priority");
		}
	}
	script::tixmlCodeNode actionPoolsNode = node->getFirstChildNode("actionpools");
	if (actionPoolsNode.isValid())
	{
		script::tixmlCodeNode poolNode = actionPoolsNode.getFirstChildNode("pool");
		while (poolNode.isValid())
		{
			int id = poolNode.getInt("id");
			std::string typeStr = poolNode.getAttr("type");
			if (id >= actionPools.size()) actionPools.resize(id + 1);
			if (typeStr == "always")
				actionPools[id].actionType = ActionPool::ALWAYS_ACTION;
			if (typeStr == "single")
				actionPools[id].actionType = ActionPool::SINGLE_ACTION;
			if (typeStr == "conbine")
				actionPools[id].actionType = ActionPool::CONBINE_ACTION;
			poolNode = poolNode.getNextNode("pool");
		}

	}
	// ���ض������ֶ�Ӧ��ID
	script::tixmlCodeNode actionNameNode = node->getFirstChildNode("actionnames");
	if (actionNameNode.isValid())
	{
		script::tixmlCodeNode actionNode = actionNameNode.getFirstChildNode("action");
		while (actionNode.isValid())
		{
			std::string actionName = actionNode.getAttr("name");
			unsigned int id = actionNode.getInt("id");
			ActionPool::actionsMap[actionName] = id;
			actionNode = actionNode.getNextNode("action");
		}
	}
	script::tixmlCodeNode myindexsNode = node->getFirstChildNode("myindexs");
	if (myindexsNode.isValid())
	{
		script::tixmlCodeNode indexNode = myindexsNode.getFirstChildNode("index");
		while (indexNode.isValid())
		{
			GridIndex index;
			index.x = indexNode.getInt("x");
			index.y = indexNode.getInt("y");
			myindexs.push_back(index); // �Լ�ռ�ݵĸ�����
			indexNode = indexNode.getNextNode("index");
		}
	}
}
void MutiMonster::doAction()
{
	for (ACTIONPOOLS_ITER iter = actionPools.begin(); iter != actionPools.end();++iter)
	{
		unsigned int actionId = iter->getAction();
		if (actionId != 0)
		{
			CartoonInfo *cartoonInfo = makeMyCartoon(actionId,dir);
			if (cartoonInfo && cartoonInfo->isValid())
			{
				nowActionName = actionId;
				iter->popAction();
				putMyCartoon(cartoonInfo);
				return ;
			}
			iter->popAction();
		}
	}
	// ִ��һ���ջص�
	CCSprite::runAction(CCSequence::create(CCDelayTime::create(0.5),
					CCCallFunc::create(this, callfunc_selector(MutiMonster::nextStep)),NULL));
	
}

CartoonInfo * MutiMonster::makeMyCartoon(const unsigned int &name,int dir)
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
bool MutiMonster::isMoving()
{
	return !movePath.empty();
}
/**
 * ���ܵȿ��Խ�������Ϊ��
 */
void MutiMonster::putMyCartoon(CartoonInfo *info)
{
	if (!info) return;
	 //������
	if (isMoving())
	{
		CCPoint dest = getLocationByIndex(movePath.front().index);
		movePath.pop_front();
		runAction(this->getParent(),info,dest,getNowTarget());
	}
	else
	{
		runAction(this->getParent(),info,ccp(0,0),getNowTarget());
	}
}

// �����ƶ���
void MutiMonster::moveToUseAstar(const GridIndex &point)
{
		//ʹ��Astar �ƶ�
	GridIndex nextIndex;
	if (map->getNextPosition(getNowIndex(),point,nextIndex))
	{
		nowLocationIndex = getNowIndex();
		maybeLocationIndex = nextIndex;
		map->setBlock(maybeLocationIndex);
		map->setBlock(nowLocationIndex);
	
		MoveActionPoint movePoint;
		movePoint.actionName = "move";
		movePoint.index = nextIndex;
		movePath.push_back(movePoint);
		tryAction(movePoint.actionName);
	}
}
void MutiMonster::tryMove(const GridIndex &index)
{
	nowAstarDestination.actionName = "move";
	nowAstarDestination.index = index;
}
void MutiMonster::jumpTo(const GridIndex &point)
{
	/**
	 * �趨��·�� �˳�
	 * tryAction("move");
	 */
}

GridIndex MutiMonster::getStartMyIndex()
{
	GridIndex index;
	index.x = getNowIndex().x + myindexs.size()?myindexs.at(0).x:0;
	index.y = getNowIndex().y + myindexs.size()?myindexs.at(0).y:0;
	return index;
}
/**
 * ��鵱ǰһ������ �Ƿ���ײ
 */
bool MutiMonster::checkCollideInMap(const GridIndex& nextIndex)
{
	return map->checkCollide(nextIndex,&myindexs);
}
/**
 * ���ݵ��ȡ��ͼ��ʵ������λ��
 */
CCPoint MutiMonster::getLocationByIndex(const GridIndex &index)
{
	if (!map) return ccp(-1,-1);
	return map->getLocationByIndex(index);
}
/**
 * ��ȡ��ǰ������
 */
GridIndex MutiMonster::getNowIndex()
{
	GridIndex index =  map->getIndexByLocation(this->getPosition());
	return index;
}

/**
 * ��鵱ǰ��Ϊ���Ƿ�����Լ�
 */
bool MutiMonster::isNowAction(const std::string &name)
{
	unsigned int id = ActionPool::getActionIdByName(name);
	if (id & nowActionName) return true;
	return false;
}
MutiMonster * MutiMonster::getNowTarget()
{
	return NULL;
}
NS_CC_END