#pragma once

#include <list>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class MapClass
{
public:
	MapClass();
	~MapClass() {};

	// 地图文件保存和加载
	void saveMap(string filename);
	void loadMap(string filename);

	// 地图元素操作
	void push(Sprite* element);
	void remove(Sprite* element);

	//地图单元纹理
	static void init();
	static vector<string> mapTexture;

	//生成精灵
	Sprite* MapClass::createSprite(int texture);

	list<Sprite*> mapElement;
};