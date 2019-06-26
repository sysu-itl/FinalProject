#include "SettingScene.h"

std::string SettingScene::settingPath = FileUtils::getInstance()->getWritablePath() + "setting.xml";

Scene* SettingScene::createScene()
{
	return SettingScene::create();
}
//���������ļ�
void SettingScene::createSettingFileIfNotExit()
{
	
	if (!FileUtils::getInstance()->isFileExist(settingPath)) {
		Dictionary::createWithContentsOfFile("setting.xml")->writeToFile(settingPath.c_str() ) ;
	}
}
//���������ļ��ֵ�
Dictionary* SettingScene::getSettingDict()
{
	createSettingFileIfNotExit();
	return Dictionary::createWithContentsOfFile(settingPath.c_str() );
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}


bool SettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if (!Scene::init())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//���������ļ����ֵ�
	Dictionary* settingDic = getSettingDict();
	Dictionary* textDic = Dictionary::createWithContentsOfFile("Chinese.xml");
	oldMusicVolumnPercentage = settingDic->valueForKey("MusicVolumn")->intValue() ;
	oldEffectVolumnPercentage = settingDic->valueForKey("EffectVolumn")->intValue();
	//��������ͼƬ
	Sprite* background = Sprite::create("background.png");
	background->setContentSize(visibleSize);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, -1);
	//�������ý���ı�����
	Sprite* backPic = Sprite::create("settingBackground.png");
	backPic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backPic, 0);
	//����һ��������
	auto MusicVolumnSliderPos = Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200);
	MusicVolumnSlider = createSlider(MusicVolumnSliderPos, oldMusicVolumnPercentage);
	MusicVolumnSlider->addEventListener(CC_CALLBACK_2(SettingScene::MusicVolumnSliderEvent,this));

	//��������ƣ����
	std::string musicName = textDic->valueForKey("musicText")->getCString();
	auto MusicNameTextPos = MusicVolumnSliderPos - Vec2(150, 0);
	createLabel(MusicNameTextPos, string(musicName));

	//����ĵ�ǰ�ٷֱȣ��ұ�
	char MusicVolumnStr[10];
	sprintf(MusicVolumnStr, "%d%%", oldMusicVolumnPercentage);
	auto MusicVolumnTextPos = MusicVolumnSliderPos + Vec2(150, 0);
	MusicVolumnText = createLabel(MusicVolumnTextPos, string(MusicVolumnStr));

	//����һ��������
	auto EffectVolumnSliderPos = MusicVolumnSliderPos - Vec2(0, 150);
	EffectVolumnSlider = createSlider(EffectVolumnSliderPos, oldEffectVolumnPercentage);
	EffectVolumnSlider->addEventListener(CC_CALLBACK_2(SettingScene::EffectVolumnSliderEvent, this));


	//��������ƣ����
	std::string effectName = textDic->valueForKey("effectText")->getCString();
	auto EffectNameTextPos = EffectVolumnSliderPos - Vec2(150, 0);
	createLabel(EffectNameTextPos, string(effectName));

	//����ĵ�ǰ�ٷֱȣ��ұ�
	char EffectVolumnStr[10];
	sprintf(EffectVolumnStr, "%d%%", oldEffectVolumnPercentage);
	auto EffectVolumnTextPos = EffectVolumnSliderPos + Vec2(150, 0);
	EffectVolumnText = createLabel(EffectVolumnTextPos, string(EffectVolumnStr));

	//���ذ�ť
	std::string backText = ( (String*)textDic->valueForKey("back") )->getCString();
	auto ttf_1 = LabelTTF::create(backText, "Arial", 40);
	auto menuItem1 = MenuItemLabel::create(ttf_1, CC_CALLBACK_1(SettingScene::backButtonCallback, this));
	menuItem1->setPosition(Vec2(visibleSize.width * 3 / 4, visibleSize.height * 1 / 4 - 50));
	menuItem1->setColor(Color3B(147, 68, 0));
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);
	//�������ð�ť
	std::string saveText = ((String*)textDic->valueForKey("saveSetting"))->getCString();
	auto ttf_2 = LabelTTF::create(saveText, "Arial", 40);
	auto menuItem2 = MenuItemLabel::create(ttf_2, CC_CALLBACK_1(SettingScene::saveButtonCallback, this));
	menuItem2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
	menuItem2->setColor(Color3B(147, 68, 0));
	auto menu2 = Menu::create(menuItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);
	return true;
}
//����������
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
//������ǩ
Label* SettingScene::createLabel(Vec2 labelPos, std::string text, int fontSize, Color3B textColor)
{
	auto label = Label::createWithSystemFont(text, "Arial", fontSize);
	label->setPosition(labelPos);
	label->setColor(textColor);
	this->addChild(label, 2);
	return label;
}
//�������ֻ��������¼�
void SettingScene::MusicVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type)
{
	char volumnStr[10];
	int showVolumnPercentage = MusicVolumnSlider->getPercent();
	sprintf(volumnStr, "%d%%", showVolumnPercentage);
	MusicVolumnText->setString(string(volumnStr) );
}

//������Ч���������¼�
void SettingScene::EffectVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type)
{
	char volumnStr[10];
	int showVolumnPercentage = EffectVolumnSlider->getPercent();
	sprintf(volumnStr, "%d%%", showVolumnPercentage);
	EffectVolumnText->setString(string(volumnStr) );
}
//���ذ�ť���¼�
void SettingScene::backButtonCallback(Ref* pSender)
{
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
//���水ť���¼�
void SettingScene::saveButtonCallback(Ref* pSender)
{
	Dictionary* settingDic= Dictionary::createWithContentsOfFile(settingPath.c_str() );
	CCInteger* newMusicVolumn = CCInteger::create(MusicVolumnSlider->getPercent());
	settingDic->setObject(newMusicVolumn, "MusicVolumn");
	CCInteger* newEffectVolumn = CCInteger::create(EffectVolumnSlider->getPercent());
	settingDic->setObject(newEffectVolumn, "EffectVolumn");
	
	settingDic->writeToFile(settingPath.c_str());

}

