#include "MapScene.h"
#include "map.h"
#include "math\MathUtil.h"
#include "cocos2d.h"
#include "MenuScene.h"

#include <iostream>
USING_NS_CC;

//// Print useful error message instead of segfaulting when files are not there.
//static void problemLoading(const char* filename)
//{
//	printf("Error while loading: %s\n", filename);
//	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
//}

void MapScene::setPhysicsWorld(PhysicsWorld * world)
{
	m_physicWorld = world;
}

Scene* MapScene::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);

	// Debug 模式
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = MapScene::create();
	//设置物理世界
	layer->setPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -980));

	return scene;
}

// on "init" you need to initialize your instance
bool MapScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//初始化
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 当前选取元素
	selSprite = NULL;

	// 当前绘制元素
	drawSprite = NULL;

	// 地图实例
	map = MapClass();

	// 菜单
	this->addMenu();

	//绘制背景
	this->drawBackground();

	//绘制基本地图元素
	this->drawMapElement();

	//添加监听器
	this->addListener();

	//测试元素
	testInit();

	//loadMapButtonCallback();
	//testMenu();
	

	//temp
	printLabel = Label::create("test", "Arial", 20);
	printLabel->setPosition(visibleSize.width - 100, visibleSize.height - 100);
	printLabel->setColor(Color3B::BLACK);
	addChild(printLabel, 3);

	print("Press M to open menu");
}

void MapScene::drawBackground()
{
	background = TMXTiledMap::create("map.tmx");
	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);
	background->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(background, -1);
}

void MapScene::addMenu()
{
	//开始按钮背景
	backphoto = Sprite::create("menuButton.png");
	backphoto->setTextureRect(Rect(0, 0, 216, 380));
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//加载地图按钮
	loadLabel = Label::create("Load Map", "Arial", 35);
	loadLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, 160));

	//保存地图按钮
	saveLabel = Label::create("Save Map", "Arial", 35);
	saveLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, 80));

	//清空地图按钮
	clearLabel = Label::create("Clear Map", "Arial", 35);
	clearLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, 0));

	// 测试地图
	testLabel = Label::create("Test Map", "Arial", 35);
	testLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, -80));

	// 返回按钮
	backLabel = Label::create("Back", "Arial", 35);
	backLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, -150));


	this->addChild(backphoto);
	this->addChild(loadLabel);
	this->addChild(saveLabel);
	this->addChild(testLabel);
	this->addChild(backLabel);
	this->addChild(clearLabel);

	//隐藏
	hideMenu();
}

void MapScene::drawMapElement()
{

	float unitWidth = visibleSize.width / (MapClass::mapTexture.size() + 1);

	//地图单元绘制
	for (int i = 0; i < MapClass::mapTexture.size(); i++) {
		auto sprite = map.createSprite(i);

		sprite->setTextureRect(Rect(0, 0, board * 4, board));
		sprite->setPosition(origin + localConvert(Size(unitWidth*(MapClass::mapTexture.size() - i), visibleSize.height - sprite->getContentSize().height / 2 - board)));
		addChild(sprite);

		mapElement.emplace_back(sprite);
	}
}

void MapScene::print(string str) {
	printLabel->setString(str.c_str());
}

void MapScene::backMainPage()
{
	auto scene = MenuScene::createScene();

	Director::getInstance()->replaceScene(scene);
}

void MapScene::clearMap()
{
	for (const auto& element : map.mapElement) {
		this->removeChild(element);
	}
	map.clear();
}

void MapScene::addListener()
{
	//鼠标监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MapScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MapScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MapScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//键盘监听
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MapScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//碰撞监听
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MapScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool MapScene::onTouchBegan(Touch * touch, Event * event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);

	// 点击菜单
	if (state == DRAW_MENU) {
		if (loadLabel->getBoundingBox().containsPoint(touchLocation)) {
			loadMapButtonCallback();
		}

		else if (saveLabel->getBoundingBox().containsPoint(touchLocation)) {
			saveMapButtonCallback();
		}
		else if (testLabel->getBoundingBox().containsPoint(touchLocation)) {
			testMenu();
		}
		else if (backLabel->getBoundingBox().containsPoint(touchLocation)) {
			backMainPage();
		}
		else if (clearLabel->getBoundingBox().containsPoint(touchLocation)) {
			clearMap();
		}
		return true;
	}


	touchLocation = localConvert(touchLocation);

	if (state == DRAW_NONE || state == DRAW_READY) {
		if (isInDrawLocal(touchLocation)) {

			Sprite* selElement = NULL;
			for (auto const &sprite : map.mapElement)
			{
				if (sprite->getBoundingBox().containsPoint(touchLocation))
				{
					selElement = sprite;
					break;
				}
			}
			if (selElement != NULL) {
				drawSprite = selElement;
			}
			else if (state == DRAW_READY) {
				state = DRAW_ACTIVE;

				drawStart = touchLocation;
				drawSprite = map.createSprite(selSprite->getTag());
				drawSprite->setPosition(drawStart);
				addChild(drawSprite);
			}

		}
		else {
			for (auto const &sprite : mapElement)
			{
				if (sprite->getBoundingBox().containsPoint(touchLocation))
				{
					selSprite = sprite;
					break;
				}
			}
			if (selSprite != NULL && state == DRAW_NONE) {
				state = DRAW_READY;
			}
		}
	}

	return true;
}


bool MapScene::isInDrawLocal(const Point& loc)
{
	return loc.y >= 0 && loc.y <= visibleSize.height - board * 3;
}

void MapScene::onTouchMoved(Touch * touch, Event * event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	if (state == DRAW_ACTIVE) {
		touchLocation = localConvert(touchLocation);

		if (true) {
			float wdis = touchLocation.x - drawStart.x;
			float hdis = touchLocation.y - drawStart.y;

			if (wdis >= 0 && hdis >= 0) {
				if (wdis < hdis)
					wdis = board;
				else
					hdis = board;

				drawSprite->setTextureRect(Rect(0, 0, wdis, hdis));
			}
		}
	}
	else if (state == DRAW_NONE || state == DRAW_MOVE) {
		Point oldTouchLocation = touch->getPreviousLocation();
		oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
		Point translation = touchLocation - oldTouchLocation;

		//窗口移动
		Point winPos = this->getPosition() + translation;

		winPos.x = MIN(winPos.x, 0);
		winPos.y = this->getPosition().y;
		translation = winPos - this->getPosition();

		this->moveWin(translation);
	}
}

void MapScene::onTouchEnded(Touch * touch, Event * event)
{
	if (state == DRAW_ACTIVE) {
		if (drawSprite->getContentSize().width != 0 && drawSprite->getContentSize().height != 0) {
			map.setPhysic(drawSprite);
			map.push(drawSprite);
		}
		state = DRAW_READY;

	}
}

void MapScene::moveWin(const Point& translation)
{
	this->setPosition(this->getPosition() + translation);

	//背景移动
	background->setPosition(background->getPosition() - translation);

	//地图绘制单元移动
	for (auto& element : mapElement) {
		element->setPosition(element->getPosition() - translation);
	}

	//菜单移动
	backphoto->setPosition(backphoto->getPosition() - translation);
	saveLabel->setPosition(saveLabel->getPosition() - translation);
	loadLabel->setPosition(loadLabel->getPosition() - translation);
	testLabel->setPosition(testLabel->getPosition() - translation);
	backLabel->setPosition(backLabel->getPosition() - translation);
	clearLabel->setPosition(clearLabel->getPosition() - translation);
	printLabel->setPosition(printLabel->getPosition() - translation);
}

void MapScene::loadMapButtonCallback()
{
	CCLOG("load");
	auto temp = map.mapElement;
	if (map.loadMap("map1")) {
		for (auto element : temp) {
			this->removeChild(element);
		}
		for (auto& element : map.mapElement) {

			this->addChild(element);
		}
	}
}

void MapScene::saveMapButtonCallback()
{
	map.saveMap("map1");
}

void MapScene::onKeyPressed(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {

		//菜单
	case cocos2d::EventKeyboard::KeyCode::KEY_M:
		if (state == DRAW_MENU) {
			hideMenu();
			if (lastState == DRAW_TEST) {
				map.move();
			}
			state = lastState;
		}
		else {
			showMenu();
			lastState = state;
			if (lastState == DRAW_TEST) {
				map.stop();
				player->getPhysicsBody()->setVelocity(Vec2::ZERO);
			}
			state = DRAW_MENU;

		}

		break;

		//取消选取
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (state == DRAW_READY) {
			state = DRAW_MOVE;
		}
		break;

		//删除
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (drawSprite != NULL) {
			map.remove(drawSprite);
			this->removeChild(drawSprite);
			drawSprite = NULL;
		}
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (state == DRAW_TEST) {
			double dx = player->getPhysicsBody()->getVelocity().x;
			double dy = 480;
			canJump++;

			if (canJump <= jumpCount) {
				player->getPhysicsBody()->setVelocity(Vec2(dx, dy));
			}
		}
		break;
	default:
		break;
	}
}

void MapScene::onKeyReleased(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {
		//取消选取
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (selSprite != NULL && state != DRAW_TEST) {
			state = DRAW_READY;
		}
		break;
	}
}

void MapScene::showMenu()
{
	backphoto->setGlobalZOrder(2);
	loadLabel->setGlobalZOrder(3);
	saveLabel->setGlobalZOrder(3);
	testLabel->setGlobalZOrder(3);
	backLabel->setGlobalZOrder(3);
	clearLabel->setGlobalZOrder(3);
}

void MapScene::hideMenu()
{
	backphoto->setGlobalZOrder(-2);
	loadLabel->setGlobalZOrder(-2);
	saveLabel->setGlobalZOrder(-2);
	testLabel->setGlobalZOrder(-2);
	backLabel->setGlobalZOrder(-2);
	clearLabel->setGlobalZOrder(-2);
}

void MapScene::testMenu()
{
	if (lastState != DRAW_TEST) {
		state = DRAW_TEST;
		//player
		createPlayer();
		moveWin(-this->getPosition());
		//testlabel
		testLabel->setString("Test terminal");
		testReset();
		map.move();
	}
	else {
		state = DRAW_NONE;
		//player
		this->removeChild(player);
		//testlabel
		testLabel->setString("Test Map");

		map.reset();
	}
	
	hideMenu();
}

//测试初始化
void MapScene::testInit()
{
	gameover = false;

	//初始块
	Sprite* firstBlock = map.createSprite(0);
	firstBlock->setPosition(0, 192);
	firstBlock->setTextureRect(Rect(0, 0, 512, 32));
	map.setPhysic(firstBlock);
	map.push(firstBlock);
	addChild(firstBlock);

	//调度器
	schedule(schedule_selector(MapScene::update), 0.02f, kRepeatForever, 0.0f);
	schedule(schedule_selector(MapScene::StopAction), 0.01f, kRepeatForever, 0.0f);
}

//测试游戏player
void MapScene::createPlayer()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("run.png");
	cocos2d::Vector<SpriteFrame*> playerRun;
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
	player->runAction(run);
	player->setAnchorPoint(Vec2(0.5, 0.5));
	auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	playerBody->setCategoryBitmask(0xFFFFFFFF);
	playerBody->setCollisionBitmask(0xFFFFFFFF);
	playerBody->setContactTestBitmask(0xFFFFFFFF);
	playerBody->setRotationEnable(false);
	player->setTag(ChickenTag);
	playerBody->setDynamic(true);
	player->setPosition(Vec2(50, 230));
	player->setPhysicsBody(playerBody);

	this->addChild(player, 3);
}

void MapScene::update(float dt)
{
	if (state == DRAW_TEST) {
		double	xVelocity = visibleSize.width / 2 - player->getPosition().x,
			yVelocity = player->getPhysicsBody()->getVelocity().y;
		player->getPhysicsBody()->setVelocity(Vec2(xVelocity, yVelocity));
	}
}

//测试停止
void MapScene::StopAction(float dt)
{
	if (state == DRAW_TEST) {
		if (player->getPosition().x <= 0 || player->getPosition().y <= 0 || gameover == true) {
			testReset();
			map.move();
			gameover = false;
		}
	}
}

//测试重置
void MapScene::testReset()
{
	//player
	player->setPosition(Vec2(50, 230));

	//map
	map.reset();

	jumpCount = 1;
	canJump = 0;
}

bool MapScene::onContactBegin(PhysicsContact & contact)
{
	auto node1 = contact.getShapeA()->getBody()->getNode(),
		node2 = contact.getShapeB()->getBody()->getNode();
	if (node1 == nullptr || node2 == nullptr) {
		return false;
	}

	auto Tag1 = node1->getTag(),
		Tag2 = node2->getTag();

	// player, block
	if (pairMatch(Tag1, Tag2, ChickenTag, BlockTag)) {
		int flag = 0;

		if (Tag1 == ChickenTag) {
			if (node1->getPosition().y >= node2->getPosition().y) {
				flag = 1;
			}
		}
		else {
			if (node2->getPosition().y >= node1->getPosition().y) {
				flag = 1;
			}
		}

		canJump = flag == 1 ? 0 : canJump;
		CCLOG("block");
	}

	// player, trap
	if (pairMatch(Tag1, Tag2, ChickenTag, trapTag)) {
		gameover = true;
		CCLOG("trap");
	}

	return true;
}

bool MapScene::pairMatch(int a1, int a2, int b1, int b2)
{
	return ((a1 == b1) && (a2 == b2))
		|| ((a1 == b2) && (a2 == b1));
}
