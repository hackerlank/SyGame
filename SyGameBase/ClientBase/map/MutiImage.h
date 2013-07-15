#pragma once

#include "MutiObject.h"

NS_CC_BEGIN

class MutiImage:public MutiObject{
public:
	std::string pngName;
	static MutiImage * create();
	static MutiImage * create(script::tixmlCodeNode *node);
	/**
	 * �������ļ��ж�ȡ��Ϣ
	 */
	virtual void readNode(script::tixmlCodeNode *node);
	/**
	 * ����Ϣд��ڵ㵱��
	 */
	TiXmlElement * writeNode(TiXmlElement *parent,const std::string &name);
	
	bool checkIn(const CCPoint &point);
	void rebuild();
};

class MutiBigImage:public CCNode,public script::tixmlCode{
public:
	static MutiBigImage* create(const char *pngName);
	bool init(const char *pngName);

	void saveToXml(float width_pixel_width,const CCPoint &offset,float image_height,float height_pixel_height,const char *pngName,int width_size,int imgSize);

	static MutiBigImage* createFromXml(const  char *pngName);
	bool initFromXml(const char *pngName);

	void takeNode(script::tixmlCodeNode *node);
private:
	CCPoint offset;
};
NS_CC_END