#pragma once

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class Hint
{
public:
	static Hint* getInstance();

	void RestartHint(Vec2& origin, Size& visibleSize, Layer* defaultLayer);

	void restartButtonCallback(Ref* sender);

	void backMenuButtonCallback(Ref* sender);
private:
	Hint();
	static Hint* instance;
};