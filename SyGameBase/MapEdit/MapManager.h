#pragma once
#include "MutiMap.h"
#include "UIWindow.h"
NS_CC_BEGIN

/**
 * ��ǰ��ͼ������ 
 * �������� �� window ��ʵ��
 */
class MapManager{
public:
	static MapManager & getMe()
	{
		static MapManager mm;
		return mm;
	}
	void replaceMap(MutiMap *map);

	void addMap(MutiMap *map);
	UIWindow * getWindow();
	MutiMap *getMap();
	CCScene *getScene();
	UIWindow *window;
	MutiMap * nowMap;
	MutiMap * mapRoot;
	CCScene *scene;
	MutiMonster *nowMonster;
	MapManager()
	{
		nowMonster = NULL;
		mapRoot = NULL;
		scene = NULL;
		window = NULL;
		nowMap = NULL;
		nowObject = NULL;
		isSetBlock = false;
		blockValue = 0;
	}
	void doTouch(int,const CCPoint &point);

	void choiceMap(MutiMap *map);
	
	MutiObject *nowObject;
	CCPoint nowTouchPoint;
	
	std::list<MutiMap*> mapList;
	typedef std::list<MutiMap*>::iterator MAPLIST_ITER;

	bool isSetBlock;
	int blockValue;
};

NS_CC_END