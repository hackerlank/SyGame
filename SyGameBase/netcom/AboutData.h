#pragma once
#include "cmdobject.h"
/**
 * ������Ϣ
 */
struct stBagInfo:public CmdObject{
	int width;
	int height;
	std::string pngName;
	int uniquePngId;
	stBagInfo():CmdObject(0)
	{
		width = 0;
		height = 0;
		uniquePngId = 0;
	}
	CMD_SERIALIZE()
	{
		BIND(width);
		BIND(height);
		BIND(pngName);
		BIND(uniquePngId);
	}
};