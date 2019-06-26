#include "Test.h"
Test* Test::instance = nullptr;

Test* Test::getInstance() {
	if (instance == nullptr) {
		instance = new Test();
	}
	return instance;
}

Test::Test() {
	isTestMode = false;
}

bool Test::GetMode() {
	return isTestMode;
}

// 测试数据库
void Test::TestDatabase() {
	if (!isTestMode) return;
	// 测试生命值记录
	Database::getInstance()->setPlayerLife(0, 1);
	CCLOG("life: %d", Database::getInstance()->getPlayerLife(0));
	// 测试跳跃次数记录
	Database::getInstance()->setPlayerJumpCount(0, 2);
	CCLOG("JumpCount: %d", Database::getInstance()->getPlayerJumpCount(0));
	// 测试最高分纪录
	Database::getInstance()->setPlayerHighScore(0, 3);
	CCLOG("HighScore: %d", Database::getInstance()->getPlayerHighScore(0));
	// 测试本次得分纪录
	Database::getInstance()->setPlayerCurrentScore(0, 4);
	CCLOG("CurrentScore: %d", Database::getInstance()->getPlayerCurrentScore(0));
	// 测试排行榜记录
	Database::getInstance()->addRank(0, 1);
	Database::getInstance()->addRank(0, 8);
	Database::getInstance()->addRank(0, 4);
	std::vector<std::vector<string>> rank = Database::getInstance()->getRank();
	CCLOG("RANK LIST");
	for (int i = 0; i < rank.size(); i++) {
		CCLOG("%s %s", rank[i][0].c_str(), rank[i][1].c_str());
	}
}

// 测试音效
void Test::TestMusic(int mode) {
	if (!isTestMode) return;
	// 不同按键对应不同的mode

	switch (mode) {
	case 4:
		// 测试背景音乐播放
		Music::getInstance()->playBackgroundMusic();
		break;
	case 5:
		// 测试背景音乐暂停
		Music::getInstance()->pauseBackgroundMusic();
		break;
	case 6:
		// 测试背景音乐停止
		Music::getInstance()->stopBackgroundMusic();
		break;
	case 7:
		// 测试特效音乐停止
		Music::getInstance()->stopCurrentEffectMusic();
		break;
	default:
		// 测试特效音乐播放
		Music::getInstance()->playMusic(mode);
		break;
	}
}

// 开启测试
void Test::OpenTestMode() {
	isTestMode = true;
}
	