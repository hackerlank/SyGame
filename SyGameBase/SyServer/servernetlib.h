#pragma once
#include "netlib.h"
#include "scriptLib.h"
class NetLib;
typedef int (NetLib::*NodeAction)(ServerNode*,script::tixmlCodeNode*);

/**
 * �ڵ�ִ��
 * */
struct NetLibAction{
	int type; // ִ�е�����
	NodeAction action;  // ��Ϊ
};
/**
 * �ڵ�ִ�п�
 * */
class NetLib:public script::Lib<NetLibAction,ServerNode,NetLib>,
	public Singleton<NetLib>
{
public:
    /**
     * ֻ����scope="stub" ��ʱ�� type ����Ч
     */
    int var(ServerNode* stub,script::tixmlCodeNode * node)
    {
        return script::Lib<NetLibAction,ServerNode,NetLib>::var(stub,node);
    }
    /**
     * ���
     */
    int print(ServerNode* stub,script::tixmlCodeNode * node)
    {
    	return script::Lib<NetLibAction,ServerNode,NetLib>::print(stub,node);
    }
    /**
     * �������
     **/
    int calc(ServerNode* stub,script::tixmlCodeNode * node)
    {
    	return script::Lib<NetLibAction,ServerNode,NetLib>::calc(stub,node);
    }
    /**
     * ִ�������ڵ�
     **/
    int exec(ServerNode* stub,script::tixmlCodeNode * node)
    {
        execCode(stub,node->getAttr("name"));
        return 0;
    }
    /**
     *  ����������
     * */
    int serverStart(ServerNode* stub,script::tixmlCodeNode * node);
    /**
     * �����ͻ���
     * */
    int clientStart(ServerNode* stub,script::tixmlCodeNode * node); 

	/**
	 * ע����Ϊ
	 */
	int reg(ServerNode* stub,script::tixmlCodeNode * node);
    /**
	 * ���ص�ͼ
	 */
	int loadMap(ServerNode* stub,script::tixmlCodeNode * node);
	/**
	 * ж����ع���
	 */
	int withNo(ServerNode* stub,script::tixmlCodeNode * node);
	/**
	 * ��ָ�������� ������
	 */
	int withIp(ServerNode *stub, script::tixmlCodeNode *node);
	/**
     * ����Ϊ
     * */
     void bindActions();
};

#define theNetLib NetLib::getMe()