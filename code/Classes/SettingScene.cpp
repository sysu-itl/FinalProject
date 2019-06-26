#include "SettingScene.h"

std::string SettingScene::settingPath = FileUtils::getInstance()->getWritablePath() + "setting.xml";

Scene* SettingScene::createScene()
{
	return SettingScene::create();
}

void SettingScene::createSettingFileIfNotExit()
{
	
	if (!FileUtils::getInstance()->isFileExist(settingPath)) {
		Dictionary::createWithContentsOfFile("setting.xml")->writeToFile(settingPath.c_str() ) ;
	}
}

Dictionary* SettingScene::getSettingDict()
{
	createSettingFileIfNotExit();
	return Dictionary::createWithContentsOfFile(settingPath.c_str() );
}


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if (!Scene::init())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Dictionary* settingDic = getSettingDict();
	Dictionary* textDic = Dictionary::createWithContentsOfFile("Chinese.xml");
	oldMusicVolumnPercentage = settingDic->valueForKey("MusicVolumn")->intValue() ;
	oldEffectVolumnPercentage = settingDic->valueForKey("EffectVolumn")->intValue();

	
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


	Sprite* backPic = Sprite::create("settingBackground.png");
	backPic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backPic, 0);


	//创建一个滑块条
	auto MusicVolumnSliderPos = Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200);
	MusicVolumnSlider = createSlider(MusicVolumnSliderPos, oldMusicVolumnPercentage);
	MusicVolumnSlider->addEventListener(CC_CALLBACK_2(SettingScene::MusicVolumnSliderEvent,this));

	//滑块的名称，左边
	std::string musicName = textDic->valueForKey("musicText")->getCString();
	auto MusicNameTextPos = MusicVolumnSliderPos - Vec2(150, 0);
	createLabel(MusicNameTextPos, string(musicName));

	//滑块的当前百分比，右边
	char MusicVolumnStr[10];
	sprintf(MusicVolumnStr, "%d%%", oldMusicVolumnPercentage);
	auto MusicVolumnTextPos = MusicVolumnSliderPos + Vec2(150, 0);
	MusicVolumnText = createLabel(MusicVolumnTextPos, string(MusicVolumnStr));

	//创建一个滑块条
	auto EffectVolumnSliderPos = MusicVolumnSliderPos - Vec2(0, 150);
	EffectVolumnSlider = createSlider(EffectVolumnSliderPos, oldEffectVolumnPercentage);
	EffectVolumnSlider->addEventListener(CC_CALLBACK_2(SettingScene::EffectVolumnSliderEvent, this));


	//滑块的名称，左边
	std::string effectName = textDic->valueForKey("effectText")->getCString();
	auto EffectNameTextPos = EffectVolumnSliderPos - Vec2(150, 0);
	createLabel(EffectNameTextPos, string(effectName));

	//滑块的当前百分比，右边
	char EffectVolumnStr[10];
	sprintf(EffectVolumnStr, "%d%%", oldEffectVolumnPercentage);
	auto EffectVolumnTextPos = EffectVolumnSliderPos + Vec2(150, 0);
	EffectVolumnText = createLabel(EffectVolumnTextPos, string(EffectVolumnStr));

	//返回按钮
	std::string backText = ( (String*)textDic->valueForKey("back") )->getCString();
	auto ttf_1 = LabelTTF::create(backText, "Arial", 40);
	auto menuItem1 = MenuItemLabel::create(ttf_1, CC_CALLBACK_1(SettingScene::backButtonCallback, this));
	menuItem1->setPosition(Vec2(visibleSize.width * 3 / 4, visibleSize.height * 1 / 4 - 50));
	menuItem1->setColor(Color3B(147, 68, 0));
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);
	//保存设置按钮
	std::string saveText = ((String*)textDic->valueForKey("saveSetting"))->getCString();
	auto ttf_2 = LabelTTF::create(saveText, "Arial", 40);
	auto menuItem2 = MenuItemLabel::create(ttf_2, CC_CALLBACK_1(SettingScene::saveButtonCallback, this));
	menuItem2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
	menuItem2->setColor(Color3B(147, 68, 0));
	auto menu2 = Menu::create(menuItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);

	/*
	Dictionary* dic = Dictionary::createWithContentsOfFile("setting.xml");
	CCDouble* newVolumn = CCDouble::create(1.0);
	dic->setObject(newVolumn, "volumn");
	dic->writeToFile("setting.xml");
	*/
	return true;
}

ui::Slider* SettingScene::createSlider(Vec2 sliderPos, int oriPercentage)
{
	auto slider = ui::Slider::create();

	slider->loadBarTexture("sliderTrack.png");
	slider->loadSlidBallTextures("sliderballnormal.png", "sliderballpressed.png", "sliderThumb.png");
	slider->loadProgressBarTexture("sliderProgress.png");
	slider->setPosition(sliderPos);
	slider->setPercent(oriPercentage);
	this->addChild(slider,2);
	return slider;
}

Label* SettingScene::createLabel(Vec2 labelPos, std::string text, int fontSize, Color3B textColor)
{
	auto label = Label::createWithSystemFont(text, "Arial", fontSize);
	label->setPosition(labelPos);
	label->setColor(textColor);
	this->addChild(label, 2);
	return label;
}

void SettingScene::MusicVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type)
{
	char volumnStr[10];
	int showVolumnPercentage = MusicVolumnSlider->getPercent();
	sprintf(volumnStr, "%d%%", showVolumnPercentage);
	MusicVolumnText->setString(string(volumnStr) );
}


void SettingScene::EffectVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type)
{
	char volumnStr[10];
	int showVolumnPercentage = EffectVolumnSlider->getPercent();
	sprintf(volumnStr, "%d%%", showVolumnPercentage);
	EffectVolumnText->setString(string(volumnStr) );
}

void SettingScene::backButtonCallback(Ref* pSender)
{
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}


void SettingScene::saveButtonCallback(Ref* pSender)
{
	Dictionary* settingDic= Dictionary::createWithContentsOfFile(settingPath.c_str() );
	CCInteger* newMusicVolumn = CCInteger::create(MusicVolumnSlider->getPercent());
	settingDic->setObject(newMusicVolumn, "MusicVolumn");
	CCInteger* newEffectVolumn = CCInteger::create(EffectVolumnSlider->getPercent());
	settingDic->setObject(newEffectVolumn, "EffectVolumn");
	
	settingDic->writeToFile(settingPath.c_str());
	//MusicVolumnText->setString(settingPath.c_str() );
}

