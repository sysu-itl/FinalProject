#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "math\MathUtil.h"

#include "GameShared.h"

USING_NS_CC;
using namespace std;

class propsFactory
{
public:
	
	void setPhysicsWorld(PhysicsWorld* physicWorld);
	void setLayer(Layer *layer);
	static propsFactory* getInstance();
	
	Vector<Sprite*> createRandomProps(Sprite* block, Layer* a);
	Sprite* createGold(Vec2& pos, Layer* placeLayer = nullptr);
	Sprite* createShoe(Vec2& pos, Layer* placeLayer = nullptr);
	Sprite* createRocket(Vec2& pos, Layer* placeLayer = nullptr);
	Sprite* createMedicine(Vec2& pos, Layer* placeLayer = nullptr);

	void removeIfOut();
	void removeAll();
	void stopAll();
	void speedUp(double multi);
	void removeProps(Sprite* obj);
	list<Sprite*> getPropsList();
	void resetSpeed();
	void resetGoldPosibilityRange();
	void resetShoePosibilityRange();
	void resetMedicinePosibilityRange();
	void resetAll();

private:
	propsFactory();
	const double	oriGoldPossibilityRange = 0.3,
					oriShoePossibilityRange = 0.4,
					oriMedicinePossibilityRange = 0.5;
	

	double	shoePossibilityRange,
			goldPossibilityRange,
			medicinePossibilityRange,
			speed,
			medicineLimitSpeed = GameMinSpeed + 30;
	PhysicsWorld * m_physicsWorld = NULL;
	Layer* defaultLayer = NULL;
	Sprite* createProps(Vec2& pos , const char path[] , Layer* placeLayer );
	static propsFactory* instance;
	list<Sprite*> propsList;
};

