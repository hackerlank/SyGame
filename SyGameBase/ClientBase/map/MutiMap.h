#pragma once
#include "cocos2d.h"
#include "xmlScript.h"
#include "MutiImage.h"
#include "MutiObject.h"
#include "MutiCartoon.h"
NS_CC_BEGIN
/**
 * ����һ������ͼ
 *	<map name="" width="" height="">
 *		<points name="" str="base64code"/>
 *		<points name="" str="base64code"/>
 *		<img src=""/>
 *		<cartoon src=""/>
 *		<map name="" speedx="" speedy="" x="" y= "" zorder="">
 *		</map>
 *	</map>
 */
class MutiMap:public CCParallaxNode,public script::tixmlCode{
public:
	static MutiMap * create(script::tixmlCodeNode *node);
	/**
	 * ѡ�����
	 */
	MutiObject *pickObject(const CCPoint &pixelPoint);

	/**
	 * �������ļ��ж�ȡ��Ϣ
	 */
	virtual void readNode(script::tixmlCodeNode *node);
	/**
	 * ����Ϣд��ڵ㵱��
	 */
	virtual TiXmlElement * writeNode(TiXmlElement *parent,const std::string &name);

	void write(const std::string &name);


	static MutiMap * create(const char *fileName);
	void takeNode(script::tixmlCodeNode *node);

	void addImage(MutiImage *image);
protected:
	std::list<MutiImage*> _images; // ͼƬ����
	std::list<MutiCartoon*> _cartoons; // ��������
	std::list<MutiMap*> _grouds; // Զ�㼯��
	typedef std::list<MutiImage*>::iterator IMAGES_ITER;
	typedef std::list<MutiCartoon*>::iterator CARTOONS_ITER;
	typedef std::list<MutiMap*>::iterator GROUDS_ITER;
	int zOrder;
	CCPoint ratio;
	CCPoint offset;
};

/**
 * ��ͼ�ı༭ģʽ
 */
class MutiMapEdit:public MutiMap{
public:
protected:
	MutiMap *nowBackgroud;
	MutiImage *nowImage;
	MutiCartoon *nowCartoon;
};
NS_CC_END