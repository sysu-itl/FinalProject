#include "MapScene.h"

#include "math\MathUtil.h"
#include "cocos2d.h"

#include <iostream>
USING_NS_CC;

//// Print useful error message instead of segfaulting when files are not there.
//static void problemLoading(const char* filename)
//{
//	printf("Error while loading: %s\n", filename);
//	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
//}

Scene* MapScene::createScene()
{
	return MapScene::create();
}

// on "init" you need to initialize your instance
bool MapScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
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

	//temp
	printLabel = Label::create("test", "Arial", 20);
	printLabel->setPosition(visibleSize.width - 100, visibleSize.height - 100);
	printLabel->setColor(Color3B::BLACK);
	addChild(printLabel, 3);

	print(to_string(visibleSize.width) + "  " + to_string(visibleSize.height));
}

void MapScene::drawBackground()
{
	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);
	this->addChild(background, -1);
}

void MapScene::addMenu()
{
	//开始按钮背景
	backphoto = Sprite::create("startbutton.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//加载地图按钮
	loadLabel = Label::create("Load Map", "Arial", 35);
	loadLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)+ Vec2(0, 60));

	//保存地图按钮
	saveLabel = Label::create("Save Map", "Arial", 35);
	saveLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, -50));

	this->addChild(backphoto);
	this->addChild(loadLabel);
	this->addChild(saveLabel);

	//隐藏
	backphoto->setGlobalZOrder(-2);
	loadLabel->setGlobalZOrder(-2);
	saveLabel->setGlobalZOrder(-2);

	CCLOG("add backphoto");
}

void MapScene::drawMapElement()
{

	float unitWidth = visibleSize.width / (MapClass::mapTexture.size()+1);

	//地图单元绘制
	for (int i = 0; i < MapClass::mapTexture.size(); i++) {
		auto sprite = map.createSprite(i);

		sprite->setTextureRect(Rect(0, 0, board * 4, board));
		sprite->setPosition(origin + localConvert(Size(unitWidth*(MapClass::mapTexture.size() - i), visibleSize.height - sprite->getContentSize().height/2 - board)));
		addChild(sprite);

		mapElement.emplace_back(sprite);
	}
}

void MapScene::print(string str) {
	printLabel->setString(str.c_str());
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
}

bool MapScene::onTouchBegan(Touch * touch, Event * event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);


	if (loadLabel->getBoundingBox().containsPoint(touchLocation)) {
		loadMapButtonCallback();
		return true;
	}
		
	else if (saveLabel->getBoundingBox().containsPoint(touchLocation)) {
		saveMapButtonCallback();
		return true;
	}
		


	touchLocation = localConvert(touchLocation);
	print(to_string(touchLocation.x) + "  " + to_string(touchLocation.y));
	
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
				print(to_string(selElement->getTag()));
			}
			else if (state == DRAW_READY){
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
				print("selIndex:" + to_string(selSprite->getTag()));
			}
		}
	}

	return true;
}


bool MapScene::isInDrawLocal(const Point& loc)
{
	//Rect boardRect = Rect(origin.x, origin.y + visibleSize.height - board * 3, visibleSize.width, board * 3);
	return loc.y >= 0 && loc.y <= visibleSize.height - board * 3;
}

bool MapScene::onTouchMoved(Touch * touch, Event * event)
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
		this->setPosition(winPos);

		//背景移动
		background->setPosition(-winPos);

		//地图绘制单元移动
		for (auto& element : mapElement) {
			element->setPosition(element->getPosition()-translation);
		}

		//菜单移动
		backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) - winPos);
		saveLabel->setPosition(saveLabel->getPosition() - translation);
		loadLabel->setPosition(loadLabel->getPosition() - translation);

		print(to_string(background->getPosition().x) + "  " + to_string(this->getPosition().x));
	}

	return true;
}

bool MapScene::onTouchEnded(Touch * touch, Event * event)
{
	if (state == DRAW_ACTIVE) {
		if (drawSprite->getContentSize().width != 0 && drawSprite->getContentSize().height != 0) {
			map.push(drawSprite);
			print("add");
		}
		state = DRAW_READY;
		
	}
		
	return true;
}

void MapScene::loadMapButtonCallback()
{
	CCLOG("load");
	for (const auto& element : map.mapElement) {
		this->removeChild(element);
	}
	map.loadMap("map1");
	for (const auto& element : map.mapElement) {
		this->addChild(element);
	}
}

void MapScene::saveMapButtonCallback()
{
	CCLOG("save");
	map.saveMap("map1");
}

void MapScene::onKeyPressed(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {

	case cocos2d::EventKeyboard::KeyCode::KEY_M:
		if (backphoto->getGlobalZOrder() != -2) {
			backphoto->setGlobalZOrder(-2);
			loadLabel->setGlobalZOrder(-2);
			saveLabel->setGlobalZOrder(-2);
		}
		else {
			backphoto->setGlobalZOrder(2);
			loadLabel->setGlobalZOrder(3);
			saveLabel->setGlobalZOrder(3);
		}
		


		break;

	//取消选取
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (state == DRAW_READY) {
			state = DRAW_MOVE;
			print("state:  " + to_string(state));

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

	}
}

void MapScene::onKeyReleased(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {
		//取消选取
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (selSprite != NULL) {
			state = DRAW_READY;
			print("state:  " + to_string(state));

		}
		break;
	}
}
