#pragma once

#include <stdio.h>
#include "cocos2d.h"

#include "MenuScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class RuleScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void backButtonCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(RuleScene);
};
