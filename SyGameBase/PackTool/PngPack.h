/**
 * ����ͼƬ�ָ� СͼƬ�ϲ� �����ṩһ�µļ�������
 */
#pragma once
#include <list>
#include <vector>
#include <map>
#include "serialize.h"
#include <png.hpp>
class CCRect{
public:
	float x;
	float y;
	float width;
	float height;
	CCRect()
	{
		x = y = 0;
		width = height = 0;
	}
	CCRect(float x,float y,float width,float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	bool equals(const CCRect &rect)
	{
		return (x == rect.x &&  y == rect.y && width == rect.width && height == rect.height);
	}
};
#define CCRectZero  CCRect()
#define CCRectMake(x,y,width,height) CCRect(x,y,width,height)
/**
 * ��������
 */
class TextureIndex : public serialize::Object{
public:
	int textureId; // ������
	int frameId; // �ڲ�֡���
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(textureId,0);
		SERIALIZE_DATA(frameId,1);
	}
	TextureIndex(int textureId,int frameId)
	{
		this->textureId = textureId;
		this->frameId = frameId;
	}
	TextureIndex()
	{
		textureId = -1;
		frameId = -1;
	}
};
class PngPack;
/**
 * ��������
 */
class ObjectData: public serialize::Object{
public:
	std::string pngName; // ��Դ����
	int width_size; // ͼƬ��
	int each_pixel_width; // ÿ����Դ�Ŀ�
	int each_pixel_height; // ÿ����Դ�ĸ�
	int image_width; // ͼƬ��
	int image_height; // ͼƬ��
	std::vector<TextureIndex> frameIds; // ��Դ�б�
	typedef std::vector<TextureIndex>::iterator FRAMEIDS_ITER;
	/**
	 * object ����д�뵽png ͼƬ
	 */
	void write(PngPack *pack,png::image< png::rgba_pixel > & pngbuffer);
	
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(pngName,0);
		SERIALIZE_DATA(frameIds,1);
		SERIALIZE_DATA(width_size,2);
		SERIALIZE_DATA(each_pixel_width,3);
		SERIALIZE_DATA(each_pixel_height,4);
		SERIALIZE_DATA(image_width,5);
		SERIALIZE_DATA(image_height,6);
	}
};
/**
 * ֡����
 */
class FrameData : public serialize::Object{
public:
	int x,y,width,height;
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(x,0);
		SERIALIZE_DATA(y,1);
		SERIALIZE_DATA(width,2);
		SERIALIZE_DATA(height,3);
	}
	FrameData()
	{
		x = y = width = height = 0; 
	}
};
/**
 * ��������
 */
class TextureData:public serialize::Object{
public:
	enum {
		EACH_IMAGE_WIDTH = 256,
		EACH_IMAGE_HEIGHT = 256,
		EACH_TEXTURE_WIDTH = 512,
		EACH_TEXTURE_HEIGHT = 512,
		EACH_BLOCK_WIDTH = 32, // ÿ������Ŀ�
		EACH_BLOCK_HEIGHT = 32, // ÿ������ĸ�
	};
	int textureId; //�ڲ���id ��
	std::vector<FrameData> frames; // ͼƬ�б�Ȼ���е�֡
	std::vector<unsigned char> contents;
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(textureId,0);
		SERIALIZE_DATA(frames,1);
		SERIALIZE_DATA(contents,2);
	}
	TextureData()
	{
	}
	/**
	 * ��ȡ���ŵķ�������
	 * return ZeroRect ʱ˵�����ܷ���
	 */
	CCRect getPrefectRect(const CCRect & rect);
	/**
	 * ����ͼƬ��ָ������ ���� framesId
	 * \param img �������img
	 * \param rect �����������
	 */
	int addImgByRect(const png::image< png::rgba_pixel > &img,const CCRect & rect);

	/**
	 * ��������ͼƬ��
	 */
	void write(png::image< png::rgba_pixel > &img,int frameId,const CCRect & rect);
	void write(png::image< png::rgba_pixel > &img);
	
};
/**
 * ��png ���
 */
class PngPack :public serialize::Object{
public:
	static PngPack & getMe()
	{
		static PngPack pack;
		return pack;
	}
	/**
	 * �ϲ�ͼƬ��packϵͳ
	 */
	void addPng(const char *pngName);
	/**
	 * �ϲ�ͼƬ��packϵͳ
	 */
	void addPng(const char *pathName,const char *pngName);
	/**
	 * ���浽pack ��
	 */
	void save(const char *packName);
	/**
	 * ��pack �м���
	 */
	void load(const char *packName);
	/**
	 * ��� �����ݻָ��ɿɶ���png ͼƬ
	 */
	void unpack();
	
	void saveAllTextures();
	TextureData *getTextureByID(int txtId);
public:

	/**
	 * ��ȡ֡����
	 * \param index ��������
	 */
	FrameData * getFrameData(const TextureIndex &index);
	std::vector<TextureData> textures; // ��������
	std::map<std::string,ObjectData> objects; // ��������
	typedef std::vector<TextureData>::iterator TEXTURES_ITER;
	typedef std::map<std::string,ObjectData>::iterator OBJECTS_ITER;
	IMP_SERIALIZE_DATA_FUNC()
	{
		SERIALIZE_DATA(textures,0);
		SERIALIZE_DATA(objects,1);
	}
};