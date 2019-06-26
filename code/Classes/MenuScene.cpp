#include "MainScene.h"

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Dictionary* dic = Dictionary::createWithContentsOfFile("Chinese.xml");

	//database
	Database::getInstance()->createDatabase();
	//
	Music::getInstance()->preloadMusic();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//add background pic
	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);

	//add components of main menu page
	auto gameName = Label::createWithSystemFont("Running Chicken", "Arial", 80);
	gameName->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 75));
	gameName->setColor(Color3B(147, 68, 0));
	this->addChild(gameName, 2);

	Sprite* backphoto = Sprite::create("menuButton.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 75 ));
	backphoto->setScale(1.25, 1.25);
	this->addChild(backphoto, 1);

	Vec2 ButtonPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 1 / 2 + 175);
	Vec2 downDis = Vec2(0, -100);

	//menu start game
	String* startText = (String*)dic->valueForKey("start");
	std::string startStr = startText->getCString();
	auto startTTF = LabelTTF::create(startStr, "Arial", 35);
	auto startMenuItem = MenuItemLabel::create(startTTF, CC_CALLBACK_1(MenuScene::startButtonCallback, this));
	startMenuItem->setPosition(ButtonPos);
	startMenuItem->setColor(Color3B(130, 60, 0));
	auto startMenu = Menu::create(startMenuItem, NULL);
	startMenu->setPosition(Vec2::ZERO);
	this->addChild(startMenu, 2);

	//menu set map
	String* customMapText = (String*)dic->valueForKey("customMap");
	std::string customMapStr = customMapText->getCString();
	auto customMapTTF = LabelTTF::create(customMapStr, "Arial", 35);
	auto customMapMenuItem = MenuItemLabel::create(customMapTTF, CC_CALLBACK_1(MenuScene::customMapButtonCallback, this));
	customMapMenuItem->setPosition(ButtonPos + downDis);
	customMapMenuItem->setColor(Color3B(130, 60, 0));
	auto customMapMenu = Menu::create(customMapMenuItem, NULL);
	customMapMenu->setPosition(Vec2::ZERO);
	this->addChild(customMapMenu, 2);

	//menu rule information 
	String* ruleText = (String*)dic->valueForKey("rule");
	std::string ruleStr = ruleText->getCString();
	auto ruleTTF = LabelTTF::create(ruleStr, "Arial", 35);
	auto ruleMenuItem = MenuItemLabel::create(ruleTTF, CC_CALLBACK_1(MenuScene::ruleButtonCallback, this));
	ruleMenuItem->setPosition(ButtonPos + downDis * 2);
	ruleMenuItem->setColor(Color3B(130, 60, 0));
	auto ruleMenu = Menu::create(ruleMenuItem, NULL);
	ruleMenu->setPosition(Vec2::ZERO);
	this->addChild(ruleMenu, 2);

	//menu score rank
	String* rankText = (String*)dic->valueForKey("rank");
	std::string rankStr = rankText->getCString();
	auto rankTTF = LabelTTF::create(rankStr, "Arial", 35);
	auto rankMenuItem = MenuItemLabel::create(rankTTF, CC_CALLBACK_1(MenuScene::rankButtonCallback, this));
	rankMenuItem->setPosition(ButtonPos + downDis * 3);
	rankMenuItem->setColor(Color3B(130, 60, 0));
	auto rankMenu = Menu::create(rankMenuItem, NULL);
	rankMenu->setPosition(Vec2::ZERO);
	this->addChild(rankMenu, 2);

	//menu game setting
	String* settingText = (String*)dic->valueForKey("setting");
	std::string settingStr = settingText->getCString();
	auto settingTTF = LabelTTF::create(settingStr, "Arial", 35);
	auto settingMenuItem = MenuItemLabel::create(settingTTF, CC_CALLBACK_1(MenuScene::settingButtonCallback, this));
	settingMenuItem->setPosition(ButtonPos + downDis * 4);
	settingMenuItem->setColor(Color3B(130, 60, 0));
	auto settingMenu = Menu::create(settingMenuItem, NULL);
	settingMenu->setPosition(Vec2::ZERO);
	this->addChild(settingMenu, 2);

	//menu quit game
	String* exitText = (String*)dic->valueForKey("exit");
	std::string exitStr = exitText->getCString();
	auto exitTTF = LabelTTF::create(exitStr, "Arial", 35);
	auto exitMenuItem = MenuItemLabel::create(exitTTF, CC_CALLBACK_1(MenuScene::exitButtonCallback, this));
	exitMenuItem->setPosition(ButtonPos + downDis * 5);
	exitMenuItem->setColor(Color3B(130, 60, 0));
	auto exitMenu = Menu::create(exitMenuItem, NULL);
	exitMenu->setPosition(Vec2::ZERO);
	this->addChild(exitMenu, 2);

	return true;
}

//set callback of start game button
void MenuScene::startButtonCallback(Ref* pSender)
{
	
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//set callback of map setting
void MenuScene::customMapButtonCallback(cocos2d::Ref * pSender)
{
	auto scene = MapScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//set callback of rule information button
void MenuScene::ruleButtonCallback(Ref* pSender)
{

	auto scene = RuleScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//set callback of setting button
void MenuScene::settingButtonCallback(Ref* pSender)
{
	auto scene = SettingScene::createScene();
	Director::getInstance()->replaceScene(scene);
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//set callback of rank button
void MenuScene::rankButtonCallback(cocos2d::Ref* pSender) {
	auto scene = RankScene::createScene();
	Director::getInstance()->replaceScene(scene);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//set callback of exit button
void MenuScene::exitButtonCallback(Ref* pSender)
{
	exit(0);
}