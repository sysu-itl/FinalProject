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
	//创建随机的道具
	Vector<Sprite*> createRandomProps(Sprite* block, Layer* a);
	//创建金币
	Sprite* createGold(Vec2& pos, Layer* placeLayer = nullptr);
	//创建鞋子
	Sprite* createShoe(Vec2& pos, Layer* placeLayer = nullptr);
	//创建火箭
	Sprite* createRocket(Vec2& pos, Layer* placeLayer = nullptr);
	//创建减速药
	Sprite* createMedicine(Vec2& pos, Layer* placeLayer = nullptr);
	//移除离开场景的道具
	void removeIfOut();
	//移除所有道具
	void removeAll();
	//停止所有道具的移动
	void stopAll();
	//改变道具移动的速度
	void speedUp(double multi);
	//移除某个特定的道具
	void removeProps(Sprite* obj);
	//返回道具精灵的列表
	list<Sprite*> getPropsList();
	//重置道具的速度
	void resetSpeed();
	//重置金币生成的概率
	void resetGoldPosibilityRange();
	//重置写生成的概率
	void resetShoePosibilityRange();
	//重置药生成的概率
	void resetMedicinePosibilityRange();
	//重置
	void resetAll();

private:
	propsFactory();
	//不同道具生成的初始概率
	const double	oriGoldPossibilityRange = 0.4,
					oriShoePossibilityRange = 0.45,
					oriMedicinePossibilityRange = 0.5;
	
	//当前道具生成的概率
	double	shoePossibilityRange,
		goldPossibilityRange,
		medicinePossibilityRange;
	//当前道具的速度
	double	speed,
			medicineLimitSpeed = GameMinSpeed + 30;
	PhysicsWorld * m_physicsWorld = NULL;
	Layer* defaultLayer = NULL;
	Sprite* createProps(Vec2& pos , const char path[] , Layer* placeLayer );
	static propsFactory* instance;
	//道具精灵列表
	list<Sprite*> propsList;
};

