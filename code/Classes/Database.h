#pragma once

#include "cocos2d.h"
#include "sqlite3.h"

USING_NS_CC;
using namespace std;

class Database
{
public:
	Database();
	~Database();
	static Database* getInstance();
	void createDatabase();
	int getPlayerLife(int player);
	void setPlayerLife(int player, int life);
	int getPlayerJumpCount(int player);
	void setPlayerJumpCount(int player, int jumpCount);
	int getPlayerHighScore(int player);
	void setPlayerHighScore(int player, int high);
	int getPlayerCurrentScore(int player);
	void setPlayerCurrentScore(int player, int score);
	void resetPlayer(int player);
	void addRank(int player, int score);
	void refreshRank();
	std::vector<std::vector<string>> getRank();
	void setNewRecord(bool i);
private:
	static Database* instance;
	sqlite3* db;
	bool is_new_record;
};