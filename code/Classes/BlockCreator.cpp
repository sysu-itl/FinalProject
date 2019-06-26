#include "BlockCreator.h"

BlockCreator* BlockCreator::instance = nullptr;

BlockCreator::BlockCreator()
{
	status = 0;
	pos = Vec2::ZERO;
	size = Size::ZERO;
	maxHeigtht = 50;
	speed = GameMinSpeed;
	g = 300;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	degree = 2;
};


BlockCreator* BlockCreator::getInstance() 
{
	if (instance == nullptr) {
		instance = new BlockCreator();
	}
	return instance;
}
