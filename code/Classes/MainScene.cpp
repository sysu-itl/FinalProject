#include "MainScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "math\MathUtil.h"
#include "cocos2d.h"

#include "propsFactory.h"
#include "BlockCreator.h"
#include "Hint.h"
#include "Music.h"
#include "Database.h"



USING_NS_CC;

void MainScene::setPhysicsWorld(PhysicsWorld* world) 
{ 
	m_physicWorld = world; 
}

Scene* MainScene::createScene()
{
	srand((unsigned)time(NULL));
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);

	// Debug 模式
	if (DEBUG) {
		scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		Test::getInstance()->OpenTestMode();
	}
	auto layer = MainScene::create();
	//设置物理世界
	layer->setPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -980));
	layer->setJoint();
	propsFactory::getInstance()->setLayer(layer);
	propsFactory::getInstance()->setPhysicsWorld(scene->getPhysicsWorld());
	return scene;
}


static void problemLoading(const char* filename)
{
	
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainSceneScene.cpp\n");
}


bool MainScene::init()
{

	if (!Layer::init())
	{
		return false;
	}
	//设置初始变量
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	canJump = 0;

	jumpCount = Database::getInstance()->getPlayerJumpCount(0);
	life = Database::getInstance()->getPlayerLife(0);
	highScore = Database::getInstance()->getPlayerHighScore(0);
	Money = Database::getInstance()->getPlayerCurrentScore(0);
	Database::getInstance()->setNewRecord(false);

	//创建角色
	createPlayer();
	//创建鞋子的条
	createRestTimeBar();
	//初始化地图
	initMap();
	//创建背景
	createBackground();
	//添加监听器
	addListener();

	schedule(schedule_selector(MainScene::updateMap), 0.02f, kRepeatForever, 0.0f);
	schedule(schedule_selector(MainScene::StopAction), 0.01f, kRepeatForever, 0.0f);
	schedule(schedule_selector(MainScene::speedUp), 15.0f, kRepeatForever, 0.0f);

	//创建当前分数的label
	currentScore = Label::create("Score: 0", "Arial", 20);
	currentScore->setPosition(visibleSize.width - 100, visibleSize.height - 100);
	currentScore->setColor(Color3B::BLACK);
	addChild(currentScore, 3);
	//创建最高分的lebel
	MostHigh = Label::create("High Score: 0", "Arial", 20);
	MostHigh->setPosition(visibleSize.width - 100, visibleSize.height - 50);
	MostHigh->setColor(Color3B::BLACK);
	addChild(MostHigh, 3);

	//将最高分和分数具体值展示
	updateScoreLabel();
	updateHighScoreLabel();

	Music::getInstance()->playBackgroundMusic();
	return true;
}

//创建背景
void MainScene::createBackground() 
{
	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);
}

