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

// �������ݿ�
void Test::TestDatabase() {
	if (!isTestMode) return;
	// ��������ֵ��¼
	Database::getInstance()->setPlayerLife(0, 1);
	CCLOG("life: %d", Database::getInstance()->getPlayerLife(0));
	// ������Ծ������¼
	Database::getInstance()->setPlayerJumpCount(0, 2);
	CCLOG("JumpCount: %d", Database::getInstance()->getPlayerJumpCount(0));
	// ������߷ּ�¼
	Database::getInstance()->setPlayerHighScore(0, 3);
	CCLOG("HighScore: %d", Database::getInstance()->getPlayerHighScore(0));
	// ���Ա��ε÷ּ�¼
	Database::getInstance()->setPlayerCurrentScore(0, 4);
	CCLOG("CurrentScore: %d", Database::getInstance()->getPlayerCurrentScore(0));
	// �������а��¼
	Database::getInstance()->addRank(0, 1);
	Database::getInstance()->addRank(0, 8);
	Database::getInstance()->addRank(0, 4);
	std::vector<std::vector<string>> rank = Database::getInstance()->getRank();
	CCLOG("RANK LIST");
	for (int i = 0; i < rank.size(); i++) {
		CCLOG("%s %s", rank[i][0].c_str(), rank[i][1].c_str());
	}
}

// ������Ч
void Test::TestMusic(int mode) {
	if (!isTestMode) return;
	// ��ͬ������Ӧ��ͬ��mode

	switch (mode) {
	case 4:
		// ���Ա������ֲ���
		Music::getInstance()->playBackgroundMusic();
		break;
	case 5:
		// ���Ա���������ͣ
		Music::getInstance()->pauseBackgroundMusic();
		break;
	case 6:
		// ���Ա�������ֹͣ
		Music::getInstance()->stopBackgroundMusic();
		break;
	case 7:
		// ������Ч����ֹͣ
		Music::getInstance()->stopCurrentEffectMusic();
		break;
	default:
		// ������Ч���ֲ���
		Music::getInstance()->playMusic(mode);
		break;
	}
}

// ��������
void Test::OpenTestMode() {
	isTestMode = true;
}
	