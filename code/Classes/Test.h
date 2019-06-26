#pragma once

#include "cocos2d.h"

#include "Database.h"
#include "Music.h"

USING_NS_CC;
using namespace std;

class Test{
private:
	bool isTestMode;
	static Test* instance;
public:
	Test();
	// 运行状态
	bool GetMode();
	// 测试数据库
	void TestDatabase();
	// 测试音效
	void TestMusic(int mode);
	// 开启测试
	void OpenTestMode();
	// 单例模式
	static Test* getInstance();
};