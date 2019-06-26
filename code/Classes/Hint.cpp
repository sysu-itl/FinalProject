#include "Hint.h"
#include "MainScene.h"
#include "propsFactory.h"

Hint* Hint::instance = nullptr;

Hint::Hint() {

};

Hint* Hint::getInstance() {
	if (instance == nullptr) {
		instance = new Hint();
	}
	return instance;
};

void Hint::RestartHint(Vec2& origin, Size& visibleSize, Layer* defaultLayer) {

	Sprite* backphoto = Sprite::create("hint.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	backphoto->setScale(0.5f);
	defaultLayer->addChild(backphoto, 2);

	Dictionary* dic = Dictionary::createWithContentsOfFile("Chinese.xml");
	String* strchinese1 = (String*)dic->valueForKey("restartTitle");
	String* strchinese2 = (String*)dic->valueForKey("restart");
	String* strchinese3 = (String*)dic->valueForKey("backMenu");
	
	std::string str1 = strchinese1->getCString();
	std::string str2 = strchinese2->getCString();
	std::string str3 = strchinese3->getCString();

	auto title = LabelTTF::create(str1, "Arial", 50);
	title->setColor(Color3B(147, 68, 0));
	title->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));
	defaultLayer->addChild(title, 3);

	auto ttf_1 = LabelTTF::create(str2, "Arial", 40);
	auto menuItem1 = MenuItemLabel::create(ttf_1, CC_CALLBACK_1(Hint::restartButtonCallback, this));
	menuItem1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 40));
	menuItem1->setColor(Color3B::BLACK);
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	defaultLayer->addChild(menu1, 3);

	auto ttf_2 = LabelTTF::create(str3, "Arial", 25);
	auto menuItem2 = MenuItemLabel::create(ttf_2, CC_CALLBACK_1(Hint::backMenuButtonCallback, this));
	menuItem2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
	menuItem2->setColor(Color3B::BLACK);
	auto menu2 = Menu::create(menuItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	defaultLayer->addChild(menu2, 3);
}

void Hint::restartButtonCallback(Ref* sender) {
	BlockCreator::getInstance()->clearBlock();
	propsFactory::getInstance()->resetAll();
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void Hint::backMenuButtonCallback(Ref * sender)
{
	BlockCreator::getInstance()->clearBlock();
	propsFactory::getInstance()->resetAll();
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

