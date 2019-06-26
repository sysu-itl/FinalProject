#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BlockCreator.h"
#include "Test.h"
USING_NS_CC;
using namespace std;

class MainScene : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);
	static cocos2d::Scene* createScene();

	virtual bool init();

	void scheduleRestTime(float dt);

	void createPlayer();

	void createBackground();

	void createRestTimeBar();

	void setJoint();

	void addListener();

	void removeListener();

	void explosion(Vec2 pos, float radius);

	bool onContactBegin(PhysicsContact & contact);

	bool onContactEnd(PhysicsContact & contact);

	bool pairMatch(int a1, int a2, int b1, int b2);

	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);

	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);

	void updateMap(float dt);

	void StopAction(float dt);

	void updateScoreLabel();

	int getCurrentScore();

	void updateHighScoreLabel();
	//
	void initMap();

	void contactGold();
	
	void contactShoe();

	void contactMedicine();
	
	void speedUp(float dt);

	void changeSpeed(double multi);

	void resetShoePosibilityRange(float dt);
	
	void resetMedicinePosibilityRange(float dt);
	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);
private:
	PhysicsWorld * m_physicWorld;
	Layer* worldLayer;

	Vec2 origin;
	Size visibleSize;
	float brickSize = 13;
	int canJump;
	bool isStop = false;
	float playerleftOfRightVel = 500;
	Sprite* player;
	std::list<Sprite*> blockList;
	cocos2d::Vector<SpriteFrame*> playerRun;
	float playerVelocity;
	const float	maxSpeed = 200,
				minSpeed = 800;

	int jumpCount;
	int Money;
	int highScore;
	int life;
	int difficulty = 0;
	float shoeTime = 15;


	// 标签
	Label* blocknum;
	Label* MostHigh;
	Label* currentScore;
	ProgressTimer* restTimeProgressTimer;
};



#endif // __HELLOWORLD_SCENE_H__
