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
	//���������ļ�
	static void createSettingFileIfNotExit();
	//���������ļ��ֵ�
	static Dictionary* getSettingDict();

	virtual bool init();
	//����������
	ui::Slider* SettingScene::createSlider(Vec2 sliderPos, int oriPercentage);
	//����һ���label
	Label* createLabel(Vec2 labelPos, std::string text, int fontSize = 30, Color3B textColor = Color3B(147, 68, 0));
	//�������ֻ��������¼�
	void MusicVolumnSliderEvent(Ref* pSender, ui::Slider::EventType type);
	//������Ч���������¼�
	void EffectVolumnSliderEvent(Ref * pSender, ui::Slider::EventType type);
	//���ذ�ť���¼�
	void backButtonCallback(cocos2d::Ref* pSender);
	//���水ť���¼�
	void saveButtonCallback(Ref* pSender);

	CREATE_FUNC(SettingScene);
private:
	//����·��
	static std::string settingPath;
	Label	*MusicVolumnText,
			*EffectVolumnText;
	ui::Slider	*MusicVolumnSlider,
				*EffectVolumnSlider;
	//���ֺ���Ч�İٷֱ�
	int oldMusicVolumnPercentage,
		oldEffectVolumnPercentage;
};