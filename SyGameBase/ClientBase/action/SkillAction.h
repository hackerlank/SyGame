#pragma once
#include "cocos2d.h"
#include "Cartoon.h"
#include "FileSet.h"
#include "LocalSprite.h"
NS_CC_BEGIN
/**
 * ������Ϊ ������Ϊ ���߱�����Ϊ
 **/
class SkillActionInfo;
 /**
 * �����ƶ����� �����������Լ����� 
 * ����֡���ƶ������
 */
class SkillMoveAction:public CCMoveTo{
public:
	
	virtual void update(float time);
	bool isTempTarget;
	SkillMoveAction()
	{
		isTempTarget = false;
	}

	/**
	 * ��������
	 */
	static SkillMoveAction* create(const SkillActionInfo &cartoonInfo, const CCPoint& position);
	/**
	 * ��ʼ��cartoon �ƶ���Ϊ
	 */
	bool init(const SkillActionInfo &SkillActionInfo, const CCPoint& position);
	/**
	 * �ͷ��Լ���ռ�е�֡
	 */
	void stop(void);
	
protected:
	SkillActionInfo cartoonInfo;
};
class SkillActionInfo{
public:
	SkillActionInfo *nextInfo; // ��һ��������Ϣ
	std::string actionName; // ��������
	int delayTime; // ��ʱ
	int nextType; // ��һ������
	bool self; // �Ƿ�������
	std::string endCodeName; // ����ʱ�ص�����
	std::vector<CCSpriteFrame*> frames; // ֡����
	CCPoint offset; // ����ƶ��ķ���
	float rotate; // �����鷴ת�ĽǶ�
	int frameType; // ֡���� 
	bool follow; // ����
	bool move; // �ƶ�
	bool stop; // ��ֹ
	float needTime;// ���ĵ�ʱ��
	enum{
		FRAME_TYPE_TIME_ANIMATION, // ʱ��֡
		FRAME_TYPE_LOCATION_ACTION, // λ��֡
	};
	enum{
		NEXT_TYPE_SEQUENCE, // ��һ���뵱ǰ����
		NEXT_TYPE_TOGATHER, // ��һ���뵱ǰ����
	};
	SkillActionInfo()
	{
		frameType = FRAME_TYPE_TIME_ANIMATION;
		needTime = 1;
		self = false;
		follow = false;
		stop = false;
		delayTime = 0;
		nextInfo = NULL;
	}
	CCAnimation *createAnimation(float costtime = 0)
	{
		CCAnimation* animation = CCAnimation::create();
		for (int i = 0; i < frames.size(); i++)
		{
			animation->addSpriteFrame(frames[i]);
		}
		if (frames.empty()) return NULL;
		if (!costtime)
			animation->setDelayPerUnit(needTime / frames.size());
		else
			animation->setDelayPerUnit(costtime / frames.size());
		animation->setRestoreOriginalFrame(true);
		return animation;
	}
	CCFiniteTimeAction * createAction(CCNode *self,CCNode *parent,CCNode *target,const CCPoint &point = ccp(-1,-1),float needTime = 0,stCollideTargetCallback *callback = NULL)
	{
		CCFiniteTimeAction *action = NULL;
		CCAnimation * animation = NULL;
		if (frameType == FRAME_TYPE_TIME_ANIMATION)
		{
			if (follow)
			{
				FollowAnimationAction * followAction = FollowAnimationAction::create(target,needTime);;
				followAction->isTempTarget = true;
				self = false;
				followAction->callback = callback;
				animation = createAnimation(needTime);
    
				action = followAction;
			}
			if (stop)
			{
				// ��ֹ����
				CartoonAction * cartoonAction = CartoonAction::create(createAnimation(needTime));
				action = cartoonAction;
				if (!self)
				{
					cartoonAction->isTempTarget = true;
				}
			}
			if (move)
			{
				SkillMoveAction *moveAction = SkillMoveAction::create(*this,offset);
				action = moveAction;
			}
		}
		if (!action) return NULL;

		if (!self)
		{
				CCSprite * temp = CCSprite::create();
				if (temp)
				{
					parent->addChild(temp);
					if (animation)
					{
						temp->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
					}
					temp->setPosition(self->getPosition());
					action->setTarget(temp);
				}
		}
		return action;
	}
};
 /**
  * <Config>
		<frames fileset="">
			<frame name=""/>
			<frame name=""/>
		</frames>
  * </Config>
  **/
class CartoonFromPack:public CCObject{
public:
	bool full(SkillActionInfo *info)
	{
		this->info = info;
		return true;
	}
	static CartoonFromPack* create(const std::string &name)
	{
		CartoonFromPack *pack = new CartoonFromPack();
		pack->init(name);
		pack->autorelease();
		return pack;
	}

	bool init(const std::string &name)
	{
		return false;
	}
	void takeNode(script::tixmlCodeNode *node)
	{
		if (node->equal("Config"))
		{
			script::tixmlCodeNode framesNode = node->getFirstChildNode("frames");
			initNode(&framesNode);
		}
	}
	SkillActionInfo *info;
	void initNode(script::tixmlCodeNode *node)
	{
		if (node->equal("frames"))
		{
			std::string fileSetName = node->getAttr("fileset");
			FileSet fileSet;
			fileSet.createFromFile(fileSetName.c_str()); // �������ļ��л�ȡ��Ϣ
			script::tixmlCodeNode frameNode = node->getFirstChildNode("frame");
			while (frameNode.isValid())
			{
				LocalSpriteFrame *frame = LocalSpriteFrame::create(frameNode.getAttr("name"),&fileSet);
				info->frames.push_back(frame);
				frame->retain();
				frameNode = frameNode.getNextNode("frame");
			}
			fileSet.destroy();
		}
	}
};
/**
 * ÿ���Ӽ��ܶ��������ص�
 */
class ChildSkillEndAction:public CCCallFunc{
public:
	static ChildSkillEndAction* create(CCNode *self,SkillActionInfo *info)
	{
		ChildSkillEndAction *pRet = new ChildSkillEndAction();
        pRet->autorelease();
        return pRet;
    }
	virtual void execute()
	{
		// ����ȥ���ýű�
	}
	CCNode *self;
	SkillActionInfo*info;
	ChildSkillEndAction()
	{
		self = NULL;
		info = NULL;
	}
};

/**
 * ָ���ļ�
 * <skill name="" >
 *			<action cartoonbin="" delaytime=""  self="false" endcode="����ʱ�Ĵ�������" startcode="��ʼʱ�Ĵ�������" collide="true"/>
 *			<action cartoonbin="" delaytime="" nexttype="sequence" self="false" endcode="����ʱ�Ĵ�������" startcode="��ʼʱ�Ĵ�������" collide="true"/>
 * </skill>
 */
/**
 * ������Ϊ
 **/
class SkillAction:public CCObject{
public:
	SkillActionInfo*info; // �����ö�����Ϣ
	std::string skillName;
	SkillAction()
	{
		info = NULL;
	}
	static SkillAction* create(const std::string &skillName)
	{
		SkillAction *skillAction = new SkillAction();
		skillAction->init(skillName);
		skillAction->autorelease();
		return skillAction;
	}
	bool init(const std::string &skillName)
	{
		return true;
	}
	void initNode(script::tixmlCodeNode *node)
	{
		if (node->equal("skill"))
		{
			skillName = node->getAttr("name");
			script::tixmlCodeNode actionNode = node->getFirstChildNode("action");
			SkillActionInfo *preInfo = NULL;
			while (actionNode.isValid())
			{
				SkillActionInfo*info = new SkillActionInfo();
				std::string binName  = actionNode.getAttr("cartoonbin");
				CartoonFromPack::create(binName)->full(info);
				info->delayTime = actionNode.getInt("delaytime");
				info->self = actionNode.getBool("self");
				info->stop = actionNode.getBool("stop");
				info->follow = actionNode.getBool("follow");
				info->needTime = actionNode.getInt("needtime");
				std::string nextTypeStr = actionNode.getAttr("nexttype");
				if (nextTypeStr == "sequence")
				{
					info->nextType = SkillActionInfo::NEXT_TYPE_SEQUENCE;
				}
				else
				{
					info->nextType = SkillActionInfo::NEXT_TYPE_TOGATHER;
				}
				if (!preInfo) // ����Ϣ��������
				{
					preInfo = info;
					this->info = info;
				}
				else
				{
					preInfo->nextInfo = info;
					preInfo = info;
				}
					
				actionNode = actionNode.getNextNode("action");
			}
		}
	}
	void runAction(CCNode *self,const cocos2d::CCPoint &point,CCNode *target,int needTime,stCollideTargetCallback * callback)
	{
		if (!info) return;
		SkillActionInfo * root =(SkillActionInfo*) info;
		int nextConbine = root->nextType;
		CCFiniteTimeAction *preAction = NULL;
		CCFiniteTimeAction *nowAction = NULL;
		while(root)
		{
			nowAction = root->createAction(self,self->getParent(),target,point,needTime,callback); // ��ײpack
			if (!preAction)
			{
				preAction = CCSequence::create(nowAction,NULL);
			}
			else
			{
				switch(nextConbine)
				{
					case CartoonInfo::SEQUENCE:
					{
						preAction = CCSequence::create(preAction,nowAction,NULL); 
					}break;
					case CartoonInfo::TOGETHER:
					{
						preAction = CCSpawn::create(preAction,nowAction,NULL);
					}break;
				}
				if (root->delayTime)
				{
					preAction = CCSequence::create(CCDelayTime::create(root->delayTime),preAction,NULL);// ����delay
				}
				preAction = CCSequence::create(preAction,
					ChildSkillEndAction::create(self,root),
					NULL); // �����ص�
			}
			nextConbine = root->nextType;
			root = root->nextInfo;
		}
		if (preAction)
		{
			CCFiniteTimeAction *seqaction = CCSequence::create(preAction,/*CCDelayTime::create(0.5),*/
						CCCallFunc::create(this, callfunc_selector(Cartoon::doCartoonEnd)),NULL);
		
			if (seqaction)
			{
				self->runAction(seqaction);
			}
		}
	}
};


void SkillMoveAction::update(float time)
{
	if (m_pTarget)
	{
		m_pTarget->setPosition(ccp(m_startPosition.x + m_delta.x * time,
			m_startPosition.y + m_delta.y * time));
		CCPoint nowPoint = m_pTarget->getPosition();
		float dw = ccpDistance(nowPoint,m_startPosition);
		float da = ccpDistance(m_endPosition,m_startPosition);
		int frameSize = this->cartoonInfo.frames.size();
		int nowFrameId = frameSize - 1;
		if (da) nowFrameId = frameSize * (dw / da);
		if (nowFrameId >= 0 && nowFrameId < frameSize)
		{
			CCSpriteFrame *frame = this->cartoonInfo.frames[nowFrameId];
			if (frame)
			{
				CCSprite *sprite = static_cast<CCSprite*>(m_pTarget);
				if (sprite)
					sprite->setDisplayFrame(frame);
			}
		}
	}
}
	
SkillMoveAction* SkillMoveAction::create(const SkillActionInfo &cartoonInfo, const CCPoint& position)
{
	SkillMoveAction *pMove = new SkillMoveAction();
	pMove->init(cartoonInfo,position);
    pMove->autorelease();
    return pMove;
}
/**
* ��ʼ��cartoon �ƶ���Ϊ
*/
bool SkillMoveAction::init(const SkillActionInfo &cartoonInfo, const CCPoint& position)
{
	this->cartoonInfo = cartoonInfo;
	initWithDuration(cartoonInfo.needTime, position);
	return true;
}

void SkillMoveAction::stop(void)
{
	CCNode *tempTarget = m_pTarget; 
	CCMoveTo::stop();
	cartoonInfo.release();
	if (isTempTarget)
	{
		tempTarget->removeFromParentAndCleanup(true);
	}
}
NS_CC_END