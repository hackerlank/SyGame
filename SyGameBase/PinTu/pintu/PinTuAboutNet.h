#pragma once
#include "AboutRequest.h"
class ClientNode;
/**
 * ��Ϣ���ĳ�
 */
class AboutNet:public MessageCenter,public Singleton<AboutNet>{
public:
    AboutNet(){}
    ~AboutNet(){destroy();}
    void init();
	bool connect();
	Socket * getSocket();
	ClientNode * client;
};
extern AboutNet theNet ;
