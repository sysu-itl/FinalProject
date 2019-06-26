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

	if (!Scene::init())
	{
		return false;
	}
	//获得中文词典
	Dictionary* dic = Dictionary::createWithContentsOfFile("Chinese.xml");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//总的背景
	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);
	//排行榜背景
	Sprite* backphoto = Sprite::create("hint.png");
	backphoto->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backphoto, 1);
	//生成返回按钮
	String* backText = (String*)dic->valueForKey("back");
	std::string backStr = backText->getCString();
	auto backTTF = LabelTTF::create(backStr, "Arial", 40);
	auto backMenuItem = MenuItemLabel::create(backTTF, CC_CALLBACK_1(RankScene::backButtonCallback, this));
	backMenuItem->setPosition(Vec2(visibleSize.width * 3 / 4, visibleSize.height * 1 / 4 - 50));
	backMenuItem->setColor(Color3B(147, 68, 0));
	auto backMenu = Menu::create(backMenuItem, NULL);
	backMenu->setPosition(Vec2::ZERO);
	this->addChild(backMenu, 1);

	//标题的label
	Vec2 titlePosition(visibleSize.width / 2, visibleSize.height - 150);
	String* titleText = (String*)dic->valueForKey("rank");
	std::string titleStr = titleText->getCString();
	createLabel(titlePosition, titleStr, 40);
	//排行榜内容的位置
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
//创建一般的label
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