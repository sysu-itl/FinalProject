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
	// 错误输出
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
	//添加轮询定时器
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
	//创建剩余条
	auto sprite = Sprite::create("restTimeBar.png"); 
	//创建progress对象
	restTimeProgressTimer = ProgressTimer::create(sprite);
	//类型：条状
	restTimeProgressTimer->setType(ProgressTimer::Type::BAR);
	restTimeProgressTimer->setScale(0.08, 0.12);
	restTimeProgressTimer->setAnchorPoint(Vec2(0.5, 0.5));
	restTimeProgressTimer->setPosition(Point(xPos, yPos));
	restTimeProgressTimer->setMidpoint(Point(0, 0.5));
	restTimeProgressTimer->setBarChangeRate(Point(1, 0));
	this->addChild(restTimeProgressTimer);
	//刷新函数，每隔0.1秒
	schedule(schedule_selector(MainScene::scheduleRestTime), 0.1f); 
	//设置剩余条的物理属性
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

//更新鞋子的剩余条
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

//玩家吃到奖励物（鞋子）的处理函数
void MainScene::contactShoe()
{
	jumpCount++;
	Music::getInstance()->playMusic(3);
	restTimeProgressTimer->setPercentage(100);
}

//玩家吃到奖励物（金币）的处理函数
void MainScene::contactGold()
{
	Money += 5;
	updateScoreLabel();
	Music::getInstance()->playMusic(2);
}

//玩家吃到奖励物（药）的处理函数
void MainScene::contactMedicine()
{
	double slowSpeed = -5;
	changeSpeed(slowSpeed);
}

//重置鞋子的出现概率
void MainScene::resetShoePosibilityRange(float dt)
{
	propsFactory::getInstance()->resetShoePosibilityRange();
}

//重置药出现的概率
void MainScene::resetMedicinePosibilityRange(float dt)
{
	propsFactory::getInstance()->resetMedicinePosibilityRange();
}

//游戏终止的处理函数
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

//游戏场景的更新函数
void MainScene::updateMap(float dt) 
{
	if (isStop) {
		return;
	}

	double	xVelocity = origin.x + visibleSize.width / 2 - player->getPosition().x,
			yVelocity = player->getPhysicsBody()->getVelocity().y;
	player->getPhysicsBody()->setVelocity(Vec2(xVelocity, yVelocity));
	
	// 设置超过最高分后记分板变红
	if (Money > highScore) {
		currentScore->setColor(Color3B::RED);
	}

	int rand = random() % 100;
	int mode = 0;
	int style1, style2;
	
	// 根据难度调节不同砖块出现的概率
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
	
	// 移除超出屏幕的奖励物品和砖块
	propsFactory::getInstance()->removeIfOut();
	BlockCreator::getInstance()->removeBlocksIfOut();
}

// 添加监听器
void MainScene::addListener() 
{
	// 监听键盘输入
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	// 监听物理碰撞
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(MainScene::onContactEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

// 移除监听器
void MainScene::removeListener() 
{
	_eventDispatcher->removeAllEventListeners();
}



// 碰撞开始的检测函数
bool MainScene::onContactBegin(PhysicsContact & contact) 
{
	auto node1 = contact.getShapeA()->getBody()->getNode(), 
		node2 = contact.getShapeB()->getBody()->getNode();
	if (node1 == nullptr || node2 == nullptr) {
		return false;
	}
	/*
	* player 1
	* block 2
	* gold 100
	* shoe 101
	* medicine 103
	*/
	auto Tag1 = node1->getTag(), 
		Tag2 = node2->getTag();


	// player, block
	if (pairMatch(Tag1, Tag2, ChickenTag, BlockTag)) {
		int flag = 0;
	
		if (Tag1 == 1) {
			if (node1->getPosition().y >= node2->getPosition().y) {
				flag = 1;
			}
		}
		else {
			if (node2->getPosition().y >= node1->getPosition().y) {
				flag = 1;
			}
		}

		canJump = flag == 1? 0 : canJump;
	}

	// player, gold
	if (pairMatch(Tag1, Tag2, ChickenTag, GoldTag)) {

		if (Tag1 == 1) {
			propsFactory::getInstance()->removeProps((Sprite*)node2);
			node2->removeFromParentAndCleanup(true);
		}
		else {
			propsFactory::getInstance()->removeProps((Sprite*)node1);
			node1->removeFromParentAndCleanup(true);
		}
		contactGold();
		return false;
	}
	// player, shoe
	if (pairMatch(Tag1, Tag2, ChickenTag, ShoeTag)) {
		if (Tag1 == 1) {
			propsFactory::getInstance()->removeProps((Sprite*)node2);
			node2->removeFromParentAndCleanup(true);
		}
		else {
			propsFactory::getInstance()->removeProps((Sprite*)node1);
			node1->removeFromParentAndCleanup(true);
		}
		contactShoe();
		scheduleOnce(schedule_selector(MainScene::resetShoePosibilityRange), 15.0f);
		return false;
	}
	
	// player, medicine
	if (pairMatch(Tag1, Tag2, ChickenTag, MedicineTag)) {
		if (Tag1 == 1) {
			propsFactory::getInstance()->removeProps((Sprite*)node2);
			node2->removeFromParentAndCleanup(true);
		}
		else {
			propsFactory::getInstance()->removeProps((Sprite*)node1);
			node1->removeFromParentAndCleanup(true);
		}
		contactMedicine();
		scheduleOnce(schedule_selector(MainScene::resetMedicinePosibilityRange), 15.0f);
		return false;
	}
	else {
		return true;
	}

	
}

// 碰撞结束的检测函数
bool MainScene::onContactEnd(PhysicsContact & contact) 
{
	auto node1 = contact.getShapeA()->getBody()->getNode(), 
		node2 = contact.getShapeB()->getBody()->getNode();
	if (node1 == nullptr || node2 == nullptr) {
		return false;
	}
	/*
	* player 1
	* block 2
	* gold 100
	* shoe 101
	* rocket 102
	* medicine 103
	*/
	auto Tag1 = node1->getTag(), Tag2 = node2->getTag();

	// player, block
	if (pairMatch(Tag1, Tag2, ChickenTag, BlockTag)) {

		return false;
	}
	else {
		return true;
	}
}

// 碰撞物体配对检测
bool MainScene::pairMatch(int a1, int a2, int b1, int b2) 
{
	return ((a1 == b1) && (a2 == b2)) 
				|| ((a1 == b2) && (a2 == b1));
}

// 键盘按下输入处理函数
void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) 
{
	double dx = player->getPhysicsBody()->getVelocity().x;
	double dy = 0;
	double disX = 300, disY = 480;
	switch (code) {
		case  cocos2d::EventKeyboard::KeyCode::KEY_W:
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			dy = disY;
			canJump++;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_0:
			if (DEBUG) {
				Test::getInstance()->TestMusic(0);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_1:
			if (DEBUG) {
				Test::getInstance()->TestMusic(1);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_2:
			if (DEBUG) {
				Test::getInstance()->TestMusic(2);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_3:
			if (DEBUG) {
				Test::getInstance()->TestMusic(3);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_4:
			if (DEBUG) {
				Test::getInstance()->TestMusic(4);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_5:
			if (DEBUG) {
				Test::getInstance()->TestMusic(5);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_6:
			if (DEBUG) {
				Test::getInstance()->TestMusic(6);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_7:
			if (DEBUG) {
				Test::getInstance()->TestMusic(7);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_8:
			if (DEBUG) {
				Test::getInstance()->TestDatabase();
			}
			break;
		default:
			break;
	}
	
	if ( canJump <= jumpCount) {
		player->getPhysicsBody()->setVelocity(Vec2(dx, dy));
		if (!DEBUG) {
			Music::getInstance()->playMusic(1);
		}
	}
}

// 键盘释放按键处理函数
void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) 
{
	double dy = player->getPhysicsBody()->getVelocity().y;
	
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		break;
	default:
		break;
	}
}

// 更新记分板显示
void MainScene::updateScoreLabel() 
{
	char str[20];
	sprintf(str, "Score: %d", Money);
	currentScore->setString(str);
}

// 更新最高分显示
void MainScene::updateHighScoreLabel() 
{
	char str[20];
	sprintf(str, "High Score: %d", highScore);
	MostHigh->setString(str);
}

// 更新当前分数
int MainScene::getCurrentScore() 
{
	return Money;
}

// 游戏提速，提高游戏难度
void MainScene::speedUp(float dt) 
{
	if (!isStop) {
		difficulty += difficulty >= 10 ? 0 : 1;
		double multi = 1.1;
		changeSpeed(multi);
	}
}

// 更改场景中的物品的移动速度
void MainScene::changeSpeed(double multi) 
{
	BlockCreator::getInstance()->speedUp(multi);
	propsFactory::getInstance()->speedUp(multi);
}