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

//创建鞋子的剩余条
void MainScene::createRestTimeBar()
{
	auto	xPos = player->getPosition().x,
			yPos = player->getPosition().y + 30;
	auto sprite = Sprite::create("restTimeBar.png");  //创建剩余条
	restTimeProgressTimer = ProgressTimer::create(sprite); //创建progress对象
	restTimeProgressTimer->setType(ProgressTimer::Type::BAR);        //类型：条状
	restTimeProgressTimer->setScale(0.08, 0.12);
	restTimeProgressTimer->setAnchorPoint(Vec2(0.5, 0.5));
	restTimeProgressTimer->setPosition(Point(xPos, yPos));
	restTimeProgressTimer->setMidpoint(Point(0, 0.5));
	restTimeProgressTimer->setBarChangeRate(Point(1, 0));
	this->addChild(restTimeProgressTimer);
	schedule(schedule_selector(MainScene::scheduleRestTime), 0.1f);  //刷新函数，每隔0.1秒

	auto restTimeProgressTimerBody = PhysicsBody::createBox(restTimeProgressTimer->getContentSize()
		, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	restTimeProgressTimerBody->setCategoryBitmask(0x0);
	restTimeProgressTimerBody->setCollisionBitmask(0x0);
	restTimeProgressTimerBody->setContactTestBitmask(0x0);

	restTimeProgressTimerBody->setDynamic(true);
	restTimeProgressTimer->setPhysicsBody(restTimeProgressTimerBody);
}
//设置人物和鞋子条的关节
void MainScene::setJoint()
{
	auto joint1 = PhysicsJointPin::construct(restTimeProgressTimer->getPhysicsBody()
		, player->getPhysicsBody(), restTimeProgressTimer->getPosition());
	joint1->setCollisionEnable(false);
	m_physicWorld->addJoint(joint1);
}
//更新鞋子的条
void MainScene::scheduleRestTime(float dt) 
{
	if (restTimeProgressTimer->getPercentage() == 0) {
		jumpCount = 1;
	}
	else if (!isStop) {
		auto percentage = restTimeProgressTimer->getPercentage();
		percentage -= dt * 100 / shoeTime;
		restTimeProgressTimer->setPercentage(percentage);
	}

}
//创建人物
void MainScene::createPlayer() 
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("run.png");
	playerRun.reserve(6);
	for (int i = 0; i < 6; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 128, 0, 120, 120)));
		playerRun.pushBack(frame);
	}
	//创建动画
	Animation* anim = Animation::createWithSpriteFrames(playerRun, 0.08f);
	Animate* ani = Animate::create(anim);
	auto run = RepeatForever::create(ani);

	player = Sprite::create("run0.png");
	player->setScale(0.3f, 0.3f);
	player->setPosition(Vec2(50, 230));
	player->runAction(run);
	player->setAnchorPoint(Vec2(0.5, 0.5));
	auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	playerBody->setCategoryBitmask(0xFFFFFFFF);
	playerBody->setCollisionBitmask(0xFFFFFFFF);
	playerBody->setContactTestBitmask(0xFFFFFFFF);
	playerBody->setRotationEnable(false);
	player->setTag(ChickenTag);
	playerBody->setDynamic(true);
	player->setPhysicsBody(playerBody);
	this->addChild(player, 2);
}
//创建初始的地图
void MainScene::initMap() { 
	auto blockPos = Vec2(origin.x, origin.y + 200);
	auto blockSize = Size(visibleSize.width, 20);
	auto block = BlockCreator::getInstance()->createBlock(0, blockPos, blockSize);
	this->addChild(block);
}

void MainScene::contactShoe()
{
	jumpCount++;
	Music::getInstance()->playMusic(3);
	restTimeProgressTimer->setPercentage(100);
}

void MainScene::contactGold()
{
	Money += 5;
	updateScoreLabel();
	Music::getInstance()->playMusic(2);
}

void MainScene::contactMedicine()
{
	double slowSpeed = -5;
	changeSpeed(slowSpeed);
}

void MainScene::resetShoePosibilityRange(float dt)
{
	propsFactory::getInstance()->resetShoePosibilityRange();
}

void MainScene::resetMedicinePosibilityRange(float dt)
{
	propsFactory::getInstance()->resetMedicinePosibilityRange();
}

void MainScene::StopAction(float dt) 
{
	if (isStop) {
		return;
	}
	
	if (player->getPosition().x <= player->getContentSize().width / 2 * 0.1 
		|| player->getPosition().y <= player->getContentSize().height / 2 * 0.1) {
		isStop = true;
		player->stopAllActions();
		player->getPhysicsBody()->setVelocity(Vec2(0, 500));
		player->getPhysicsBody()->setCategoryBitmask(0x00000000);

		if (!DEBUG) {
			removeListener();

			if (Money > highScore) {
				highScore = Money;
				Database::getInstance()->setPlayerHighScore(0, highScore);
				updateHighScoreLabel();
			}
			Database::getInstance()->addRank(0, Money);
			//Database::getInstance()->refreshRank();
		}
		Music::getInstance()->playMusic(0);
		propsFactory::getInstance()->stopAll();
		BlockCreator::getInstance()->stopBlock();
		Music::getInstance()->stopBackgroundMusic();
		Hint::getInstance()->RestartHint(origin, visibleSize, this);
	}
	
}

void MainScene::updateMap(float dt) 
{
	if (isStop) {
		return;
	}

	double	xVelocity = origin.x + visibleSize.width / 2 - player->getPosition().x,
			yVelocity = player->getPhysicsBody()->getVelocity().y;
	player->getPhysicsBody()->setVelocity(Vec2(xVelocity, yVelocity));

	if (Money > highScore) {
		currentScore->setColor(Color3B::RED);
	}

	int rand = random() % 100;
	int mode = 0;
	int style1, style2;

	switch (difficulty) {
	case 0:
		style1 = 30;
		style2 = 10;
		break;
	case 1:
		style1 = 35;
		style2 = 12;
		break;
	case 2:
		style1 = 40;
		style2 = 14;
		break;
	case 3:
		style1 = 45;
		style2 = 16;
	case 4:
		style1 = 50;
		style2 = 18;
		break;
	case 5:
		style1 = 55;
		style2 = 20;
		break;
	case 6:
		style1 = 60;
		style2 = 22;
		break;
	case 7:
		style1 = 65;
		style2 = 24;
		break;
	case 8:
		style1 = 70;
		style2 = 26;
		break;
	case 9:
		style1 = 75;
		style2 = 28;
		break;
	case 10:
		style1 = 80;
		style2 = 30;
		break;
	default:
		style1 = 30;
		style2 = 10;
		break;
	}

	if (rand >= style1) {
		mode = 0;
	}
	else if (rand >= style2) {
		mode = 1;
	}
	else {
		mode = 2;
	}

	auto block = BlockCreator::getInstance()->getBlock(mode, origin, visibleSize);

	if (block != NULL) {
		Money++;
		updateScoreLabel();
		if (mode != 2) {
			propsFactory::getInstance()->createRandomProps(block, this);
		}
		this->addChild(block);
	}

	propsFactory::getInstance()->removeIfOut();
	BlockCreator::getInstance()->removeBlocksIfOut();
}