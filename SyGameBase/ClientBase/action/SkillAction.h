#pragma once
#include "cocos2d.h"
#include "Cartoon.h"
#include "FileSet.h"
#include "LocalSprite.h"
NS_CC_BEGIN
/**
 * ������Ϊ ������Ϊ ���߱�����Ϊ
 **/
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
		move = false;
		delayTime = 0;
		nextInfo = NULL;
	}
	void release(){}
	CCAnimation *createAnimation(float costtime = 0);
	CCFiniteTimeAction * createAction(CCNode *self,CCNode *parent,CCNode *target,const CCPoint &point = ccp(-1,-1),float needTime = 0,stCollideTargetCallback *callback = NULL);
};
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
 /**
  * <Config>
		<frames fileset="">
			<frame name=""/>
			<frame name=""/>
		</frames>
  * </Config>
  **/
class CartoonFromPack:public CCObject,public script::tixmlCode{
public:
	static CartoonFromPack* create(SkillActionInfo *info,const std::string &name)
	{
		CartoonFromPack *pack = new CartoonFromPack();
		pack->info = info;
		pack->init(name);
		pack->autorelease();
		return pack;
	}
	static CartoonFromPack* create(SkillActionInfo *info,script::tixmlCodeNode *node)
	{
		CartoonFromPack *pack = new CartoonFromPack();
		pack->info = info;
		pack->initNode(node);
		pack->autorelease();
		return pack;
	}
	bool init(const std::string &name)
	{
		return script::tixmlCode::initWithXmlFile(name.c_str());
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
      //  pRet->autorelease();
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
class SkillAction:public CCObject,public script::tixmlCode{
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
	//	skillAction->autorelease();
		return skillAction;
	}
	bool init(const std::string &skillName)
	{
		return initWithXmlFile(skillName.c_str());
	}
	void takeNode(script::tixmlCodeNode *node)
	{
		if (node->equal("Config"))
		{
			script::tixmlCodeNode skillNode = node->getFirstChildNode("skill");
			initNode(&skillNode);
		}
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
				script::tixmlCodeNode framesNode = actionNode.getFirstChildNode("frames");
				CartoonFromPack::create(info,&framesNode);
				info->delayTime = actionNode.getInt("delaytime");
				info->self = actionNode.getBool("self");
				info->stop = actionNode.getBool("stop");
				info->move = actionNode.getBool("move");
				info->follow = actionNode.getBool("follow");
				info->needTime = actionNode.getInt("needtime");
				std::string nextTypeStr = actionNode.getAttr("nexttype");
				info->offset.x = actionNode.getInt("offsetx");
				info->offset.y = actionNode.getInt("offsety");
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
	void runAction(CCNode *self,const cocos2d::CCPoint &point = ccp(-1,-1),CCNode *target = NULL,int needTime=1.0f,stCollideTargetCallback * callback = NULL)
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
					case SkillActionInfo::NEXT_TYPE_SEQUENCE:
					{
						preAction = CCSequence::create(preAction,nowAction,NULL); 
					}break;
					case SkillActionInfo::NEXT_TYPE_TOGATHER:
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
			CCFiniteTimeAction *seqaction = CCSequence::create(preAction,
					/*CCDelayTime::create(0.5),*/
						/*CCCallFunc::create(this, callfunc_selector(Cartoon::doCartoonEnd)),*/
						NULL);
		
			if (seqaction)
			{
				self->runAction(seqaction);
			}
		}
	}
};

NS_CC_END