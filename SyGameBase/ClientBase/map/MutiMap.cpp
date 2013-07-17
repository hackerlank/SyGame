#include "MutiMap.h"

NS_CC_BEGIN
MutiMap * MutiMap::create(script::tixmlCodeNode *node)
{
	MutiMap *map = new MutiMap();
	if (map)
	{
		map->readNode(node);
		map->autorelease();
		return map;
	}
	CC_SAFE_DELETE(map);
	return NULL;
}
MutiMap * MutiMap::create(const char *fileName)
{
	MutiMap *map = new MutiMap();
	if (map)
	{
		std::string startui = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName);
		unsigned long nSize = 0;
		unsigned char * buffer = CCFileUtils::sharedFileUtils()->getFileData(startui.c_str(),"rb",&nSize);
		if (!nSize)return false;
		map->initFromString((char*)buffer);
		map->fileName = fileName;
		map->autorelease();
		return map;
	}
	CC_SAFE_DELETE(map);
	return NULL;
}
void  MutiMap::takeNode(script::tixmlCodeNode *node)
{
	if (node && node->equal("Config"))
	{
		script::tixmlCodeNode mapNode = node->getFirstChildNode("map");
		readNode(&mapNode);
	}
}
/**
 * ѡ�����
 */
MutiObject *MutiMap::pickObject(const CCPoint &pixelPoint)
{
	if (!this->isVisible() || isHide) return NULL;
	for (IMAGES_ITER iter = _images.begin(); iter != _images.end();++iter)
	{
		if (*iter && (*iter)->checkIn(pixelPoint))
		{
			return *iter;
		}
	}
	for (CARTOONS_ITER iter = _cartoons.begin(); iter != _cartoons.end();++iter)
	{
		if (*iter && (*iter)->checkIn(pixelPoint))
		{
			return *iter;
		}
	}
	for (BIG_IMAGES_ITER iter = _bigImages.begin();iter != _bigImages.end();++iter)
	{
		if (*iter && (*iter)->checkIn(pixelPoint))
		{
			return *iter;
		}
	}
	for (BIG_TERRAINS_ITER iter = _bigTerrains.begin();iter != _bigTerrains.end();++iter)
	{
		if (*iter && (*iter)->checkIn(pixelPoint))
		{
			return *iter;
		}
	}
	return NULL;
}
void MutiMap::execEachBg(stExecEachBackgroud *bg)
{
	bg->exec(this);
	for (GROUDS_ITER iter = _grouds.begin();iter != _grouds.end();++iter)
	{
		bg->exec(*iter);
	}
}
/**
 * �������ļ��ж�ȡ��Ϣ
 */
void MutiMap::readNode(script::tixmlCodeNode *node)
{
	script::tixmlCodeNode &mapNode = *node;
	if (mapNode.isValid())
	{
		zOrder = mapNode.getInt("zorder");
		ratio.x = mapNode.getFloat("rationx");
		ratio.y = mapNode.getFloat("rationy");
		offset.x = mapNode.getInt("offsetx");
		offset.y = mapNode.getInt("offsety");
		fileName = mapNode.getAttr("name");
		if (mapNode.isValid())
		{
			script::tixmlCodeNode imageNode = mapNode.getFirstChildNode("image");
			while(imageNode.isValid())
			{
				MutiImage *image = MutiImage::create(&imageNode);
				if (image)
				{
					CCNode::addChild(image);
					_images.push_back(image);
				}
				imageNode = imageNode.getNextNode("image");
			}
			script::tixmlCodeNode cartoonNode = mapNode.getFirstChildNode("cartoon");
			while(cartoonNode.isValid())
			{
				MutiCartoon *cartoon = MutiCartoon::create(&cartoonNode);
				if (cartoon)
				{
					CCNode::addChild(cartoon);
					_cartoons.push_back(cartoon);
				}
				cartoonNode = cartoonNode.getNextNode("cartoon");
			}
			script::tixmlCodeNode bigImageNode = mapNode.getFirstChildNode("bigimage");
			while(bigImageNode.isValid())
			{
				MutiBigImage *image = MutiBigImage::create(&bigImageNode);
				if (image)
				{
					CCNode::addChild(image);
					_bigImages.push_back(image);
				}
				bigImageNode = bigImageNode.getNextNode("bigimage");
			}
			script::tixmlCodeNode bigTerrainNode = mapNode.getFirstChildNode("bigterrain");
			while(bigTerrainNode.isValid())
			{
				MutiBigTerrain *image = MutiBigTerrain::create(&bigTerrainNode);
				if (image)
				{
					CCNode::addChild(image);
					_bigTerrains.push_back(image);
				}
				bigTerrainNode = bigTerrainNode.getNextNode("bigterrain");
			}
			/**
			 * ��ȡ�赲����Ϣ
			 */
			script::tixmlCodeNode blockNode = mapNode.getFirstChildNode("blocks");
			if (blockNode.isValid())
			{
				script::tixmlCodeNode indexNode = blockNode.getFirstChildNode("index");
				while (indexNode.isValid())
				{
					GridIndex index;
					index.x = indexNode.getInt("x");
					index.y = indexNode.getInt("y");
					setBlock(index);
					indexNode = indexNode.getNextNode("index");
				}
			}
			/**
			 * �ݹ�ķ�ʽ�����ӽڵ�
			 */
			script::tixmlCodeNode childMapNode = mapNode.getFirstChildNode("map");
			while (childMapNode.isValid())
			{
				MutiMap *map = MutiMap::create(&childMapNode);
				if (map)
				{
					this->addChild(map,map->zOrder,map->ratio,map->offset);
					_grouds.push_back(map);
				}
				childMapNode = childMapNode.getNextNode("map");
			}

		}
	}
}
void MutiMap::addImage(MutiImage *image)
{
	CCNode::addChild(image);
	_images.push_back(image);
}
void MutiMap::addCartoon(MutiCartoon *cartoon)
{
	CCNode::addChild(cartoon);
	_cartoons.push_back(cartoon);
}
void MutiMap::addMap(MutiMap *map)
{
	this->addChild(map,map->zOrder,map->ratio,map->offset);
	_grouds.push_back(map);
}
void MutiMap::addBigImage(MutiBigImage *bigImage)
{
	CCNode::addChild(bigImage);
	_bigImages.push_back(bigImage);
}
void MutiMap::addSprite(CCSprite *sprite)
{
	CCNode::addChild(sprite);
}
struct stWriteGrids:stExecEach<int>{
	void exec(const GridIndex& index)
	{
		int * value = grids->getObjectByIndex(index);
		if (value && *value == 1)
		{
			TiXmlElement *indexNode=new TiXmlElement("index");
			if (blockNode)
				blockNode->LinkEndChild(indexNode);
			indexNode->SetAttribute("x",index.x);
			indexNode->SetAttribute("y",index.y);
		}
	}
	HexagonGrids<int> * grids;
	TiXmlElement  *blockNode;
	stWriteGrids(HexagonGrids<int> * grids,TiXmlElement  *blockNode):grids(grids),blockNode(blockNode)
	{}
};
/**
* ����Ϣд��ڵ㵱��
*/
TiXmlElement * MutiMap::writeNode(TiXmlElement *parent,const std::string &name)
{
	TiXmlElement *mapNode=new TiXmlElement(name.c_str());
	if (parent)
		parent->LinkEndChild(mapNode);
	mapNode->SetAttribute("zorder",zOrder);
	mapNode->SetDoubleAttribute("rationx",ratio.x);
	mapNode->SetDoubleAttribute("rationy",ratio.y);
	mapNode->SetAttribute("offsetx",offset.x);
	mapNode->SetAttribute("offsety",offset.y);
	mapNode->SetAttribute("name",fileName);
	for (IMAGES_ITER iter = _images.begin(); iter != _images.end();++iter)
	{
		if (*iter)
			(*iter)->writeNode(mapNode,"image");
	}
	for (CARTOONS_ITER iter = _cartoons.begin(); iter != _cartoons.end();++iter)
	{
		if (*iter)
			(*iter)->writeNode(mapNode,"cartoon");
	}
	for (GROUDS_ITER iter = _grouds.begin(); iter != _grouds.end();++iter)
	{
		if (*iter)
			(*iter)->writeNode(mapNode,"map");
	}
	for (BIG_IMAGES_ITER iter = _bigImages.begin(); iter != _bigImages.end();++iter)
	{
		if (*iter)
			(*iter)->writeNode(mapNode,"bigimage");
	}
	for (BIG_TERRAINS_ITER iter = _bigTerrains.begin(); iter != _bigTerrains.end();++iter)
	{
		if (*iter)
			(*iter)->writeNode(mapNode,"bigterrain");
	}
	/**
	 * д���赲��
	 */
	TiXmlElement *blockNode=new TiXmlElement("blocks");
	if (mapNode)
		mapNode->LinkEndChild(blockNode);
	stWriteGrids exec(_grids,blockNode);
	if (_grids)
		_grids->execAll(&exec);
	return mapNode;
}

/**
 * ����ͼ���浽�ļ�
 */
void MutiMap::write(const std::string &name)
{
	TiXmlDocument *pDoc=new TiXmlDocument; //����һ���ĵ���ָ��
	//���һ��xmlͷ��
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
	pDoc->LinkEndChild(pDeclaration);
	//���XMl�ĸ��ڵ�
	TiXmlElement *configNode= new TiXmlElement("Config");
	pDoc->LinkEndChild(configNode);
	
	TiXmlElement *mapNode = writeNode(configNode,"map");

	pDoc->SaveFile(name);
}

void MutiMap::hide()
{
	isHide = true;
	for (IMAGES_ITER iter = _images.begin(); iter != _images.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(false);
	}
	for (CARTOONS_ITER iter = _cartoons.begin(); iter != _cartoons.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(false);
	}
	for (GROUDS_ITER iter = _grouds.begin(); iter != _grouds.end();++iter)
	{
		if (*iter)
			(*iter)->hide();
	}
	for (BIG_IMAGES_ITER iter = _bigImages.begin(); iter != _bigImages.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(false);
	}
	for (BIG_TERRAINS_ITER iter = _bigTerrains.begin(); iter != _bigTerrains.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(false);
	}
}
void MutiMap::show()
{
	isHide = false;
	for (IMAGES_ITER iter = _images.begin(); iter != _images.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(true);
	}
	for (CARTOONS_ITER iter = _cartoons.begin(); iter != _cartoons.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(true);
	}
	for (GROUDS_ITER iter = _grouds.begin(); iter != _grouds.end();++iter)
	{
		if (*iter)
			(*iter)->show();
	}
	for (BIG_IMAGES_ITER iter = _bigImages.begin(); iter != _bigImages.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(true);
	}
	for (BIG_TERRAINS_ITER iter = _bigTerrains.begin(); iter != _bigTerrains.end();++iter)
	{
		if (*iter)
			(*iter)->setVisible(true);
	}
}
struct stShowEachGrids:stExecEach<int>{
	void exec(const GridIndex& index)
	{
		CCSprite * test = CCSprite::create("cell.png");
		test->setPosition(grids->getPointByIndex(index));
		map->addSprite(test);
	}
	HexagonGrids<int> * grids;
	MutiMap *map;
	stShowEachGrids(HexagonGrids<int> * grids,MutiMap *map):grids(grids),map(map)
	{}
};
struct stCheckValid:public stCheckMoveAble{
	bool exec(const GridIndex &index) 
	{
		int * value = grids->getObjectByIndex(index);
		if (!value || *value == 1)
		{
			return false;
		}
		return true;
	}
	stCheckValid(HexagonGrids<int> * grids):grids(grids)
	{}
	HexagonGrids<int> * grids;
};
void MutiMap::showGrids()
{
	if (!_grids) _grids = new AStarSeachInHexagonGrids<int>(10,10,64);
	stShowEachGrids exec(_grids,this);
//	_grids->execAll(&exec);
	setBlock(GridIndex(2,2));
	setBlock(GridIndex(3,2));
	stCheckValid check(_grids);
	/**
	 * ����Ѱ·�㷨
	 */
	GridIndex out;
	_grids->getNextGridIndex(GridIndex(0,0),GridIndex(5,2),out,&exec,&check);
}

/**
 * �����赲����Ϣ
 */
void MutiMap::setBlock(const GridIndex &index)
{
	if (_grids)
	{
		int * value = _grids->getObjectByIndex(index);
		if (value)
		{
			*value = 1;
		}
	}
}
void MutiMap::setBlockByTouchPoint(const CCPoint &touchPoint)
{
	CCPoint point = this->convertToNodeSpace(touchPoint);
	GridIndex index = _grids->getIndexByPoint(point);
	setBlock(index);
	stShowEachGrids exec(_grids,this);
	_grids->execOne(index,&exec);
}
NS_CC_END