#include "propsFactory.h"

propsFactory* propsFactory::instance = nullptr;


propsFactory::propsFactory() 
{
	resetGoldPosibilityRange();
	resetShoePosibilityRange();
	resetMedicinePosibilityRange();
	resetSpeed();

}


void propsFactory::setPhysicsWorld(PhysicsWorld* physicWorld)
{
	m_physicsWorld = physicWorld;
}

void propsFactory::setLayer(Layer * layer)
{
	defaultLayer = layer;
}

propsFactory* propsFactory::getInstance() {
	if (instance == nullptr) {
		instance = new propsFactory();
	}
	return instance;
}

Vector<Sprite*> propsFactory::createRandomProps(Sprite* block , Layer* layer)
{
	Vector<Sprite*> props;
	if(block == NULL || m_physicsWorld == NULL){
		return props;
	}
	float randNum = random(0.0f, 1.0f);
	Vec2 midPos = block->getPosition();
	Vec2 frontPos = block->getPosition();
	
	midPos.y += block->getContentSize().height ;
	midPos.x += block->getContentSize().width / 2 ;
	frontPos.y += block->getContentSize().height ;

	if (randNum < goldPossibilityRange) {
		props = { createGold(midPos, layer) };
	}
	else if (randNum < shoePossibilityRange) {
		props = { createShoe(midPos, layer) };
	}
	else if (randNum < medicinePossibilityRange && speed > medicineLimitSpeed) {
		props = { createMedicine(midPos, layer) };
	}
	
	for each (auto prop in props)
	{
		Vec2 propPos = prop->getPosition();
		float randHeight = random() % 75 + 25;

		propPos.y += prop->getContentSize().height / 2 + randHeight;
		prop->setPosition(propPos);
		prop->getPhysicsBody()->setVelocity(Vec2(-speed, 0));

		//CCLOG("%f %f", prop->getPosition().x, prop->getPosition().y);
	}
	return props;
}

Sprite* propsFactory::createGold(Vec2& pos, Layer* placeLayer)
{
	auto sprite = createProps(pos, "gold.png", placeLayer);
	sprite->setTag(GoldTag);
	return sprite;
}

Sprite* propsFactory::createShoe(Vec2& pos , Layer* placeLayer)
{
	auto sprite = createProps(pos , "shoe.png" , placeLayer);
	//shoePossibilityRange = -1;
	sprite->setTag(ShoeTag);
	return sprite;
}

Sprite* propsFactory::createRocket(Vec2& pos, Layer * placeLayer)
{
	auto sprite = createProps(pos, "rocket.png", placeLayer);
	sprite->setTag(RocketTag);
	return sprite;
}

Sprite * propsFactory::createMedicine(Vec2 & pos, Layer * placeLayer)
{
	auto sprite = createProps(pos, "medicine.png", placeLayer);
	//medicinePossibilityRange = -1;
	sprite->setTag(MedicineTag);
	return sprite;
}


Sprite* propsFactory::createProps(Vec2& pos, const char path[] , Layer* placeLayer)
{
	auto sprite = Sprite::create(path);
	sprite->setPosition(pos);
	auto playerBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(100.0f, 1.0f, 0.0f));
	playerBody->setCategoryBitmask(0xFFFFFFFF);
	playerBody->setCollisionBitmask(0x0);
	playerBody->setContactTestBitmask(0xFFFFFFFF);
	playerBody->setDynamic(false);
	sprite->setPhysicsBody(playerBody);
	if (placeLayer == nullptr) {
		defaultLayer->addChild(sprite);
	}
	else {
		placeLayer->addChild(sprite);
	}
	propsList.push_back(sprite);
	return sprite;

}

list<Sprite*> propsFactory::getPropsList() {
	return propsList;
}

void propsFactory::resetSpeed()
{
	speed = GameMinSpeed;
}

void propsFactory::resetGoldPosibilityRange()
{
	goldPossibilityRange = oriGoldPossibilityRange;
}

void propsFactory::resetShoePosibilityRange()
{
	shoePossibilityRange = oriShoePossibilityRange;
}

void propsFactory::resetMedicinePosibilityRange()
{
	medicinePossibilityRange = oriMedicinePossibilityRange;
}

void propsFactory::resetAll()
{
	removeAll();
	resetGoldPosibilityRange();
	resetShoePosibilityRange();
	resetMedicinePosibilityRange();
}

void propsFactory::removeIfOut() {
	if (defaultLayer != NULL && !propsList.empty()) {
		auto block = propsList.front();
		if (block->getPosition().x <= 0) {
			block->removeFromParentAndCleanup(true);
			propsList.pop_front();
		}
	}
	else {
		//do nothing
	}
}

void propsFactory::removeAll() {
	for (auto i = propsList.begin(); i != propsList.end(); i++) {
		(*i)->removeFromParentAndCleanup(true);
	}
	propsList.clear();
}

void propsFactory::stopAll() {
	resetSpeed();
	for (auto i = propsList.begin(); i != propsList.end(); i++) {
		(*i)->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
}

void propsFactory::speedUp(double multi) {
	
	this->speed += multi * SpeedChangeRate;
	this->speed = min(this->speed, GameMaxSpeed);
	this->speed = max(this->speed, GameMinSpeed);
	for (auto i = propsList.begin(); i != propsList.end(); i++) {
		(*i)->getPhysicsBody()->setVelocity(Vec2(-this->speed, 0));
	}
}

void propsFactory::removeProps(Sprite* obj) {
	if (obj != NULL) {
		propsList.remove(obj);
	}
}