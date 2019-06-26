#pragma once
#include <stdio.h>
#include "cocos2d.h"

#include "MainScene.h"
#include "MapScene.h"
#include "RuleScene.h"
#include "SettingScene.h"
#include "RankScene.h"
#include "Database.h"
#include "SimpleAudioEngine.h"
#include "Music.h"

USING_NS_CC;
class MenuScene : public cocos2d::Scene
{
public:
	//create menu scene
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void startButtonCallback(cocos2d::Ref* pSender);
	void customMapButtonCallback(cocos2d::Ref* pSender);
	void ruleButtonCallback(cocos2d::Ref* pSender);
	void settingButtonCallback(cocos2d::Ref* pSender);
	void exitButtonCallback(cocos2d::Ref* pSender);
	void rankButtonCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);
};
