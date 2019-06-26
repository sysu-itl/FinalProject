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



Sprite* BlockCreator::createBlock(int mode, Vec2& position, Size& size) 
{
	Sprite* block = nullptr;

	block = changeBlockStyle(mode, block, position, size);

	auto playerBody1 = PhysicsBody::createBox(block->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	playerBody1->setCategoryBitmask(0xFFFFFFFF);
	playerBody1->setCollisionBitmask(0xFFFFFFFF);
	playerBody1->setContactTestBitmask(0xFFFFFFFF);
	playerBody1->setDynamic(false);
	block->setPhysicsBody(playerBody1);
	block->getPhysicsBody()->setVelocity(Vec2(-speed, 0));
	
	if (mode == 2) {
		block->getPhysicsBody()->setRotationEnable(true);
		block->getPhysicsBody()->setAngularVelocity(1.0f);
	}
	
	blockList.push_back(block);
	
	return block;
}

Sprite* BlockCreator::getBlock(int mode, Vec2 pos, Size size) 
{
	if (!blockList.empty()) {
		auto block = blockList.back();
		if (block->getPosition().x + block->getContentSize().width < origin.x + visibleSize.width) {
			setNextBlock(mode);
			return createBlock(mode, Vec2(origin.x + visibleSize.width + this->pos.x, origin.y + this->pos.y), this->size);
		}
	}

	return NULL;
}

void BlockCreator::setNextBlock(int mode) 
{
	auto height = blockList.back()->getPosition().y;

	double maxh = min((float)(height +  maxHeigtht), (2.0f / 3.0f) * visibleSize.height + origin.y);
	double minh = max(0.0, height - maxHeigtht);
	double nextHeight = random(minh, maxh);
	double maxDistance = (sqrt(2 * (height + maxHeigtht - nextHeight) / 900) + 0.3) * speed;

	pos.x = maxDistance * random(0.3 * degree + 0.2, 0.3 * degree + 0.3);
	pos.y = nextHeight;
	size = Size(random(-35 * degree + 200, -80 * degree + 350) , 20);

}


//Vec2(pos.x + size.width + 200, pos.y), Size(100, 100)
void BlockCreator::removeBlocksIfOut() 
{
	if (!blockList.empty()) {
		auto block = blockList.front();
		if (block->getPosition().x + block->getContentSize().width <= 0) {
			block->removeFromParentAndCleanup(true);
			blockList.pop_front();
		}
	}
}



void BlockCreator::stopBlock() 
{
	for (auto i = blockList.begin(); i != blockList.end(); i++) {
		(*i)->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
}

void BlockCreator::clearBlock() 
{
	this->speed = GameMinSpeed;
	blockList.clear();
}

std::list<Sprite*> BlockCreator::getBlockList() 
{
	return blockList;
}

void BlockCreator::speedUp(double multi) 
{
	this->speed += multi * SpeedChangeRate;
	this->speed = min(this->speed, GameMaxSpeed);
	this->speed = max(this->speed, GameMinSpeed);

	for (auto i = blockList.begin(); i != blockList.end(); i++) {
		(*i)->getPhysicsBody()->setVelocity(Vec2(-this->speed,0));
	}
}
inline Sprite* blockStyle1(Sprite* block, Vec2& position, Size& size) 
{
	block = Sprite::create("blockstyle1.png");
	block->setTag(BlockTag);
	block->setAnchorPoint(Vec2::ZERO);
	block->setContentSize(size);
	block->setPosition(position);
	return block;
}

inline Sprite* blockStyle2(Sprite* block, Vec2& position, Size& size) 
{
	block = Sprite::create("blockstyle2.png");
	block->setTag(BlockTag);
	block->setAnchorPoint(Vec2::ZERO);
	block->setContentSize(Size(50.0f,34.0f));
	block->setPosition(position);
	return block;
}

inline Sprite* blockStyle3(Sprite* block, Vec2& position, Size& size) 
{
	block = Sprite::create("blockstyle3.png");
	block->setTag(BlockTag);
	block->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	block->setContentSize(Size(50.0f, 50.0f));
	block->setPosition(position);
	return block;
}

inline Sprite* blockStyle4(Sprite* block, Vec2& position, Size& size) 
{
	return block;
}

inline Sprite* blockStyle5(Sprite* block, Vec2& position, Size& size) 
{
	return block;
}

inline Sprite* blockStyle6(Sprite* block, Vec2& position, Size& size) 
{
	return block;
}

Sprite* BlockCreator::changeBlockStyle(int mode, Sprite* block, Vec2& position, Size& size) 
{
	switch (mode)
	{
	case 0:
		return blockStyle1(block, position, size);
		break;
	case 1:
		return blockStyle2(block, position, size);
		break;
	case 2:
		return blockStyle3(block, position, size);
		break;
	case 3:
		return blockStyle4(block, position, size);
		break;
	case 4:
		return blockStyle5(block, position, size);
		break;
	case 5:
		return blockStyle6(block, position, size);
		break;
	default:
		break;
	}
	return NULL;
}

