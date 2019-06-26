#include "RuleScene.h"

Scene* RuleScene::createScene()
{
	return RuleScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}

// on "init" you need to initialize your instance
bool RuleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Dictionary* dic = Dictionary::createWithContentsOfFile("Chinese.xml");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);

	Sprite* backphoto = Sprite::create("hint.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backphoto, 1);

	Sprite* shoephoto = Sprite::create("shoe.png");
	shoephoto->setPosition(Vec2(visibleSize.width / 4, visibleSize.height * 3 / 4));
	shoephoto->setScale(2);
	this->addChild(shoephoto, 2);

	String* strchinese2 = (String*)dic->valueForKey("shoes");
	std::string str2 = strchinese2->getCString();
	auto shoeText = Label::createWithSystemFont(str2, "Arial", 35);
	shoeText->setPosition(Vec2(visibleSize.width / 2 - 20, visibleSize.height * 3 / 4));
	shoeText->setColor(Color3B(147, 68, 0));
	this->addChild(shoeText, 2);

	Sprite* moneyphoto = Sprite::create("gold.png");
	moneyphoto->setPosition(Vec2(visibleSize.width / 4, visibleSize.height * 5 / 8));
	moneyphoto->setScale(2);
	this->addChild(moneyphoto, 2);

	String* strchinese3 = (String*)dic->valueForKey("money");
	std::string str3 = strchinese3->getCString();
	auto moneyText = Label::createWithSystemFont(str3, "Arial", 35);
	moneyText->setPosition(Vec2(visibleSize.width / 2 - 20, visibleSize.height * 5 / 8));
	moneyText->setColor(Color3B(147, 68, 0));
	this->addChild(moneyText, 2);

	Sprite* timephoto = Sprite::create("restTimeBar.png");
	timephoto->setPosition(Vec2(visibleSize.width / 4, visibleSize.height * 1 / 2));
	timephoto->setScale(0.1);
	this->addChild(timephoto, 2);

	

	String* strchinese4 = (String*)dic->valueForKey("time");
	std::string str4 = strchinese4->getCString();
	auto timeText = Label::createWithSystemFont(str4, "Arial", 35);
	timeText->setPosition(Vec2(visibleSize.width / 2 + 20, visibleSize.height * 1 / 2));
	timeText->setColor(Color3B(147, 68, 0));
	this->addChild(timeText, 2);

	Sprite* medicinephoto = Sprite::create("medicine.png");
	medicinephoto->setPosition(Vec2(visibleSize.width / 4, visibleSize.height * 3 / 8));
	this->addChild(medicinephoto, 2);


	String* strchinese5 = (String*)dic->valueForKey("medicine");
	std::string str5 = strchinese5->getCString();
	auto medicineText = Label::createWithSystemFont(str5, "Arial", 35);
	medicineText->setPosition(Vec2(visibleSize.width / 2 - 70, visibleSize.height * 3 / 8));
	medicineText->setColor(Color3B(147, 68, 0));
	this->addChild(medicineText, 2);


	String* strchinese6 = (String*)dic->valueForKey("miaoshu");
	std::string str6 = strchinese6->getCString();
	auto howText = Label::createWithSystemFont(str6, "Arial", 35);
	howText->setPosition(Vec2(visibleSize.width / 2 + 20, visibleSize.height * 2 / 8));
	howText->setColor(Color3B(147, 68, 0));
	this->addChild(howText, 2);
	
	String* strchinese1 = (String*)dic->valueForKey("back");
	std::string str1 = strchinese1->getCString();
	auto ttf_1 = LabelTTF::create(str1, "Arial", 40);
	auto menuItem1 = MenuItemLabel::create(ttf_1, CC_CALLBACK_1(RuleScene::backButtonCallback, this));
	menuItem1->setPosition(Vec2(visibleSize.width * 3 / 4, visibleSize.height * 1 / 4 - 50));
	menuItem1->setColor(Color3B(147, 68, 0));
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);

	return true;
}

void RuleScene::backButtonCallback(Ref* pSender)
{

	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}