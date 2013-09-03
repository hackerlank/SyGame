#include "behavior.h"

int main()
{
	const char *content = "{\"name\":\"and\",\"childs\":\
						  [{\"name\":\"sayHello\",\"x\":10},\
						  {\"name\":\"sayHello\",\"x\":100},\
						  {\"name\":\"sayHello\",\"x\":100000},\
						  {\"name\":\"or\",\"childs\":[{\"name\":\"say\",\"x\":10}\
						  ]}\
	]}";
	ExampleLib lib;
	lib.initLogics(); // ��ʼ������
	lib.parseString(content); // �����ַ���
	lib.execute(NULL); // ִ��
	lib.show(10,10);
	Node *node = lib.getLeafAction("say"); // ��ȡ��ǰ�ӽڵ�
	lib.root->addChild(node);
	/*
	 *EditAction<ExampleLib> action(lib); // ��������
	 *action.start(); // չʾ�༭���߼�
	 */
	printf("node:%s\n",lib.toString().c_str());

	

}