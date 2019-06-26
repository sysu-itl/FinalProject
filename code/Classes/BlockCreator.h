#pragma once

#include "cocos2d.h"
#include <math.h>

#include "gameShared.h"

#include "MainScene.h"
#include "MenuScene.h"
#include "propsFactory.h"
#include "Hint.h"
USING_NS_CC;


USING_NS_CC;
using namespace std;

class BlockCreator {
public:
	static BlockCreator* getInstance();

private:
	BlockCreator();
	//Layer * defaultLayer;
	static BlockCreator* instance;
	std::list<Sprite*> blockList;

	int status;
	Vec2 pos;
	Size size;

	double maxHeigtht;
	double speed;
	double g;
	double degree;

	Vec2 origin;
	Size visibleSize;
};