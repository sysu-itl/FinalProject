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
	//创建设置文件
	static void createSettingFileIfNotExit();
	//返回设置文件字典
	static Dictionary* getSettingDict();

	virtual bool init();
	//创建滑动条
	ui::Slider* SettingScene::createSlider(Vec2 sliderPos, int oriPercentage);
	//创建一般的label
	Label* createLabel(Vec2 labelPos, std::string text, int fontSize = 30, Color3B textColor = Color3B(147, 68, 0));
	//滑动音乐滑动条的事件
	void MusicVolumnSliderEvent(Ref* pSender, ui::Slider::EventType type);
	//滑动音效滑动条的事件
	void EffectVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type);
	//返回按钮的事件
	void backButtonCallback(cocos2d::Ref* pSender);
	//保存按钮的事件
	void saveButtonCallback(Ref* pSender);

	CREATE_FUNC(SettingScene);
private:
	//配置路径
	static std::string settingPath;
	Label	*MusicVolumnText,
			*EffectVolumnText;
	ui::Slider	*MusicVolumnSlider,
				*EffectVolumnSlider;
	//音乐和音效的百分比
	int oldMusicVolumnPercentage,
		oldEffectVolumnPercentage;
};