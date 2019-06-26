#include "RankScene.h"
#include "Database.h"

Scene* RankScene::createScene()
{
	return RankScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool RankScene::init()
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

	/*
	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, -1);
	*/

	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);

	Sprite* backphoto = Sprite::create("hint.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backphoto, 1);



	String* strchinese1 = (String*)dic->valueForKey("back");
	std::string str1 = strchinese1->getCString();
	auto ttf_1 = LabelTTF::create(str1, "Arial", 40);
	auto menuItem1 = MenuItemLabel::create(ttf_1, CC_CALLBACK_1(RankScene::backButtonCallback, this));
	menuItem1->setPosition(Vec2(visibleSize.width * 3 / 4, visibleSize.height * 1 / 4 - 50));
	menuItem1->setColor(Color3B(147, 68, 0));
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);


	Vec2 titlePosition(visibleSize.width / 2, visibleSize.height - 150);
	String* titleText = (String*)dic->valueForKey("rank");
	std::string titleStr = titleText->getCString();
	createLabel(titlePosition, titleStr, 40);

	Vec2 rankPosition(visibleSize.width / 2 - 100, visibleSize.height - 200);
	std::vector<std::vector<std::string>> data = Database::getInstance()->getRank();

	for (int i = 0; i < data.size(); i++) {
		if (i >= showRankNum) {
			break;
		}

		createLabel(rankPosition, to_string(i+1));
		Vec2 scorePosition = rankPosition + Vec2(200, 0);
		createLabel(scorePosition, data[i][1]);
		rankPosition.y -= 50;
		
	}
	
	return true;
}

Label * RankScene::createLabel(Vec2 labelPos, std::string text, int fontSize, Color3B textColor)
{
	auto label = Label::createWithSystemFont(text, "Arial", fontSize);
	label->setPosition(labelPos);
	label->setColor(textColor);
	this->addChild(label, 2);
	return label;
}



void RankScene::backButtonCallback(Ref* pSender)
{

	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}