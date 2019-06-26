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

	Sprite * createBlock(int mode, Vec2 & position, Size & size);

	Sprite * getBlock(int mode, Vec2 pos, Size size);

	void setNextBlock(int mode);

	void removeBlocksIfOut();

	int getSize() { return blockList.size(); }

	void stopBlock();

	void clearBlock();

	std::list<Sprite*> getBlockList();

	void speedUp(double multi);

	Sprite* changeBlockStyle(int mode, Sprite* block, Vec2& position, Size& size);

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