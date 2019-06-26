#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "MenuScene.h"
#include "MainScene.h"

USING_NS_CC;

class RankScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//创建一般的lable
	Label* createLabel(Vec2 labelPos, std::string text, int fontSize = 30, Color3B textColor = Color3B(147, 68, 0));
	//返回按钮的点击事件
	void backButtonCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(RankScene);
private:
	int showRankNum = 8;
};