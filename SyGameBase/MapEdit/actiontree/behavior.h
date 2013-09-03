#pragma once
#include <vector>
#include <map>
#include <string>
#include "jsonObject.h"
#define _NODE_DEBUG
/**
 * ϵͳ�л�������
 */
class Env{
public:
	template<typename CLASS>
	CLASS *cast(){return (CLASS*) this;}
};
#ifdef _NODE_DEBUG
class Node;
class DebugActionShow{
public:
	virtual void actionWithDebug(Node *node) = 0; // ÿ��ִ��ʱ�ص�
	virtual void setPosition(int x,int y) = 0; // ����λ��
	virtual void showRelation(DebugActionShow *show) = 0; // չʾ����
	Node *__node__;
	DebugActionShow()
	{
		__node__ = NULL;
	}
};
#endif
/**
 * �ڵ�
 */
class Node{
public:
	std::string name;
	enum NODE_TYPE{
		NULL_NODE = 0,
		AND_NODE = 1,
		OR_NODE = 2,
		LEAF_NODE=3,
	};
	NODE_TYPE nodeType; // �ڵ�����
	Node *next; // ��һ���ڵ�
	Node *parent; // ���׽ڵ�
	Node *child; // �ӽڵ�
	Node()
	{
		next = parent = child = NULL;
		nodeType = NULL_NODE;
		valid = true;
#ifdef _NODE_DEBUG
		__debug__x__ = __debug__y__ = 0;
		extData = NULL;
#endif
	}
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env) 
	{
#ifdef _NODE_DEBUG
		if (extData)
			ext<DebugActionShow>()->actionWithDebug(this); // ÿ��ִ��ʱ�ص�
#endif
		return true;
	}
	static Node *create(const std::string &name);

	void addChild(Node *child);

	bool valid; // �ýڵ��Ƿ���Ч
	bool isValid(){return valid;} // �ڵ㿪�� 
#ifdef _NODE_DEBUG
	int __debug__x__; // ������
	int __debug__y__; // ������
	std::string descInfo; // ������Ϣ
	void *extData; // ��չ������
	template<typename CLASS>
	CLASS *ext(){return (CLASS*) extData;}

	void setPosition(int x,int y)
	{
		__debug__x__ = x;__debug__y__ = y;
		if (extData)
		ext<DebugActionShow>()->setPosition(x,y);
	}

	template<typename DebugInfo>
	void debugAttach(DebugInfo *debugInfo)
	{
		extData = debugInfo;
		debugInfo->__node__ = this;
	}
	void showRelation(Node *parent)
	{
		if (extData && parent && parent->extData)
			ext<DebugActionShow>()->showRelation(parent->ext<DebugActionShow>());
	}
#endif
};
/**
* ���ӽڵ㶼��Trueʱ ����True
*/
class AndNode:public Node{
public:
	AndNode()
	{
		nodeType = AND_NODE;
	}
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env)
	{
		Node::execute(env);
		Node *temp = child;
		while (temp)
		{
			if (!temp->execute(env))
			{
				return false;
			}
			temp = temp->next;
		}
		return true;
	}
};
/**
* ���ӽڵ���һ��Trueʱ ����True
*/
class OrNode:public Node{
public:
	OrNode()
	{
		nodeType = OR_NODE;
	}
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env)
	{
		Node::execute(env);
		Node *temp = child;
		while (temp)
		{
			if (temp->execute(env))
			{
				return true;
			}
			temp = temp->next;
		}
		return false;
	}
};

/**
* �����ڵ� ���� True or False
*/
class LeafNode:public Node{
public:
	LeafNode()
	{
		nodeType = LEAF_NODE;
	}
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env)
	{
		// ִ����չ
		return false;
	}
	std::map<std::string,std::string> attrs; // �����б�
	typedef std::map<std::string,std::string>::iterator ATTRS_ITER;
	double getDouble(const std::string &name);
	int getInt(const std::string &name);
	std::string getString(const std::string &name);
	virtual LeafNode *clone(){return NULL;}
};


template<typename LOGIC>
class LogicNode:public LeafNode{
public:
	typedef bool (LOGIC::*FUNCTION_HANDLE)(LeafNode *node,Env *env);
	FUNCTION_HANDLE handle;
	LOGIC *logic;
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env)
	{
		Node::execute(env);
		// ִ����չ
		return ((*logic).*handle)(this,env);
	}
	LogicNode(LOGIC *logic,FUNCTION_HANDLE handle):logic(logic),handle(handle){}
	virtual LeafNode *clone(){
		LogicNode<LOGIC> *obj = new LogicNode<LOGIC>(logic,handle);
		obj->name = name;
		return obj;
	}
};
/**
 * Document ��JSON �л�ȡ�ڵ���Ϣ 
 */
 class JSONDocument{
 public:
	JSONDocument()
	{
		root = NULL;
	}
	void parseString(const char *content);
	Node *root;
	/**
	 * ��ʼ���߼���
	 */
	virtual void initLogics() = 0;
	std::map<std::string,LeafNode*> leafNodes; // ϵͳ�е�Ҷ�ڵ�
	typedef std::map<std::string,LeafNode*>::iterator LEAF_NODES_ITER;
	bool execute(Env *env)
	{
		if (root) return root->execute(env);
		return false;
	}
	LeafNode *getLeafAction(const std::string &);
	/**
	 * ��root �ع�Ϊstring
	 */
	std::string toString();
	
	void show(int x,int y)
	{
		showNode(root,x,y);
	}
#ifdef _NODE_DEBUG
	template<typename LOGIC,typename PANEL>
	void attachDebugInfo(PANEL *panel)
	{
		attachNode<LOGIC,PANEL>(root,panel);
	}
	template<typename LOGIC,typename PANEL>
	void attachNode(Node *root,PANEL *panel)
	{
		Node *child = root->child;
		LOGIC *logic = LOGIC::create(root);
		panel->addChild(logic);
		if (!logic) return;
		root->debugAttach(logic);
		while (child )
		{
			attachNode<LOGIC,PANEL>(child,panel);
			child = child->next;
		}
	}
#endif
	int showNode(Node *root,int x,int y)
	{
		for (int i = 0; i < x;i++) printf(".");
		printf("%s: %d %d\n",root->name.c_str(),x,y);
#ifdef _NODE_DEBUG
		root->setPosition(x,y);
#endif
		Node *child = root->child;
		int offsety = 88;
		while (child && root->isValid())
		{
			offsety += showNode(child,x+88,y+offsety);
#ifdef _NODE_DEBUG
			child->showRelation(root); // չʾ�븸�ڵ�Ĺ���
#endif
			child = child->next;
		}
		return offsety;
	}
	JSONObject createString(Node *root);
 };
 
 class ExampleLib:public JSONDocument{
 public:
	void initLogics()
	{
		leafNodes["sayHello"] = new LogicNode<ExampleLib>(this,&ExampleLib::sayHello);
		leafNodes["say"] = new LogicNode<ExampleLib>(this,&ExampleLib::sayHello);
	}
	bool sayHello(LeafNode *node,Env *env)
	{
		printf("hello,world %s\n",node->getString("x").c_str());
		return true;
	}
 };
