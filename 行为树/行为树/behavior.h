#pragma once
#include <vector>
#include <map>
#include <string>
#include "jsonObject.h"
/**
 * ϵͳ�л�������
 */
class Env{
public:
};
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
	}
	/**
	 * ִ��
	 */
	virtual bool execute(Env *env) = 0;
	static Node *create(const std::string &name);

	void addChild(Node *child);

	bool valid; // �ýڵ��Ƿ���Ч
	bool isValid(){return valid;} // �ڵ㿪�� 
#ifdef _NODE_DEBUG
	int x; // ������
	int y; // ������
	std::string descInfo; // ������Ϣ
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
	int showNode(Node *root,int x,int y)
	{
		for (int i = 0; i < x;i++) printf(".");
		printf("%s: %d %d\n",root->name.c_str(),x,y);
#ifdef _NODE_DEBUG
		root->x = x; root->y = y;
#endif
		Node *child = root->child;
		int offsety = 10;
		while (child && root->isValid())
		{
			offsety += showNode(child,x+10,y+offsety);
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