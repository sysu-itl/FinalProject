#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"

USING_NS_CC;
class SettingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	static void createSettingFileIfNotExit();

	static Dictionary* getSettingDict();

	virtual bool init();

	ui::Slider* SettingScene::createSlider(Vec2 sliderPos, int oriPercentage);

	Label* createLabel(Vec2 labelPos, std::string text, int fontSize = 30, Color3B textColor = Color3B(147, 68, 0));

	void MusicVolumnSliderEvent(Ref* pSender, ui::Slider::EventType type);

	void EffectVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type);
	// a selector callback
	void backButtonCallback(cocos2d::Ref* pSender);

	void saveButtonCallback(Ref* pSender);
	
	
	// implement the "static create()" method manually
	CREATE_FUNC(SettingScene);
private:
	static std::string settingPath;
	Label	*MusicVolumnText,
			*EffectVolumnText;
	ui::Slider	*MusicVolumnSlider,
				*EffectVolumnSlider;
	int oldMusicVolumnPercentage,
		oldEffectVolumnPercentage;
};