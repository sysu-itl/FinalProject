#pragma once

#include <list>
#include "cocos2d.h"
#include "GameShared.h"

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
	void clear();

	//地图单元纹理
	static void init();
	static vector<string> mapTexture;

	//生成精灵
	Sprite* createSprite(int texture);

	//物理
	void setPhysic(Sprite* sprite);

	//重置
	void reset();

	//移动
	void move();

	//停止
	void stop();

	list<Sprite*> mapElement;

private:
	Point origin;
};