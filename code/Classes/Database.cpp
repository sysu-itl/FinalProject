#include "Database.h"

Database* Database::instance = nullptr;

Database* Database::getInstance() {
	// 单例模式
	if (instance == nullptr) {
		instance = new Database();
	}
	return instance;
}

Database::Database() { 
	// 初始化数据库
	db = NULL; 
	is_new_record = true; 
};

Database::~Database() { 
	// 关闭数据库连接
	sqlite3_close(db); 
	db = NULL;
}

void Database::setNewRecord(bool i) {
	// 设置是否为第一次游戏
	this->is_new_record = i;
}

void Database::createDatabase() {
	// 连接数据库
	std::string path = FileUtils::getInstance()->getWritablePath() + "playerInfo.db";
	int result = sqlite3_open(path.c_str(), &db);
	// 创建record表
	std::string sql = "create table if not exists record(player int, life int, jump int, curScore int, high int, primary key(player));";
	result = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		CCLOG("error0: %d", result);
	}
	// 创建rank表
	sql = "create table if not exists rank(player int, score int);";
	result = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		CCLOG("error1: %d",result);
	}
	// 插入游戏玩家初始化信息
	sql = "insert into record(player, life, jump, curScore, high) values (0,1,1,0,0);";
	result = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	is_new_record = result == SQLITE_OK ? true : false;
}

int Database::getPlayerLife(int player) {
	int life = 1;
	// 获取玩家当前生命值
	if (!is_new_record) {
		char** re;
		int row, col;
		char sql[100] = { 0 };
		sprintf(sql, "select life from record where player = %d;", player);
		sqlite3_get_table(db, sql, &re, &row, &col, NULL);
		life = atoi(re[1]);
		sqlite3_free_table(re);
	}
	else {
		life = 1;
	}
	return life;
}

int Database::getPlayerJumpCount(int player) {
	int jumpCount = 1;
	// 获取玩家当前可跳跃的次数
	if (!is_new_record) {
		char** re;
		int row, col;
		char sql[100] = { 0 };
		sprintf(sql, "select jump from record where player = %d;", player);
		sqlite3_get_table(db, sql, &re, &row, &col, NULL);
		jumpCount = atoi(re[1]);
		sqlite3_free_table(re);
	}
	else {
		jumpCount = 1;
	}
	return jumpCount;
}

int Database::getPlayerHighScore(int player) {
	int highScore = 0;
	// 获取玩家历史最高得分
	if (!is_new_record) {
		char** re;
		int row, col;
		char sql[100] = { 0 };
		sprintf(sql, "select high from record where player = %d;", player);
		sqlite3_get_table(db, sql, &re, &row, &col, NULL);
		highScore = atoi(re[1]);
		sqlite3_free_table(re);
	}
	else {
		highScore = 0;
	}
	return highScore;
}

int Database::getPlayerCurrentScore(int player) {
	int curScore = 0;
	// 获取玩家当前得分
	if (!is_new_record) {
		char** re;
		int row, col;
		char sql[100] = { 0 };
		sprintf(sql, "select curScore from record where player = %d;", player);
		sqlite3_get_table(db, sql, &re, &row, &col, NULL);
		curScore = atoi(re[1]);
		sqlite3_free_table(re);
	}
	else {
		curScore = 0;
	}
	return curScore;
}

void Database::setPlayerLife(int player, int life) {
	// 设置玩家当前生命值
	if (!is_new_record) {
		char sql[100] = { 0 };
		sprintf(sql, "update record set life = %d where player = %d;", life, player);
		int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (result == SQLITE_OK) {
			CCLOG("set life ok!");
		}
		else {
			CCLOG("set life fail!");
		}
	}
	else {
		// do nothing
	}
}

void Database::setPlayerJumpCount(int player, int jumpCount) {
	// 设置玩家当前最大跳跃次数
	if (!is_new_record) {
		char sql[100] = { 0 };
		sprintf(sql, "update record set jump = %d where player = %d;", jumpCount, player);
		int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (result == SQLITE_OK) {
			CCLOG("set jump count ok!");
		}
		else {
			CCLOG("set jump count fail!");
		}
	}
	else {
		// do nothing
	}
}

void Database::setPlayerHighScore(int player, int high) {
	// 设置玩家历史最高得分
	if (!is_new_record) {
		char sql[100] = { 0 };
		sprintf(sql, "update record set high = %d where player = %d;", high, player);
		int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (result == SQLITE_OK) {
			CCLOG("set high score ok!");
		}
		else {
			CCLOG("set high score fail!");
		}
	}
	else {
		// do nothing
	}
}

void Database::setPlayerCurrentScore(int player, int score) {
	// 设置玩家当前得分
	if (!is_new_record) {
		char sql[100] = { 0 };
		sprintf(sql, "update record set curScore = %d where player = %d", score, player);
		int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (result == SQLITE_OK) {
			CCLOG("set high score ok!");
		}
		else {
			CCLOG("set high score fail!");
		}
	}
	else {
		// do nothing
	}
}

void Database::resetPlayer(int player) {
	// 重置玩家信息
	if (!is_new_record) {
		char sql[100] = { 0 };
		sprintf(sql, "update record set life = 1, jump = 1, curScore = 0 where player = %d", player);
		int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
		if (result == SQLITE_OK) {
			CCLOG("reset ok!");
		}
		else {
			CCLOG("reset fail!");
		}
	}
	else {
		// do nothing
	}
}

void Database::addRank(int player, int score){
	// 将玩家的游戏得分添加到排行榜记录
	char sql[100] = { 0 };
	sprintf(sql, "insert into rank(player, score) values (%d, %d);", player, score);
	int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		CCLOG("insert rank ok!");
	}
	else {
		CCLOG("insert rank fail!");
	}
}

void Database::refreshRank(){
	// 更新排行榜，只保留前十名
	char sql[200] = { 0 };
	sprintf(sql, "delete from rank where score not in( select score from rank order by score DESC limit 0,10;);");
	int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		CCLOG("insert rank ok!");
	}
	else {
		CCLOG("insert rank fail!");
	}
}

std::vector<std::vector<string>> Database::getRank(){
	std::vector<std::vector<string>> result;
	char** re;
	int row, col;
	// 获取排行榜的信息
	std::string sql = "select player, score from rank order by score DESC limit 0,10;";
	int res = sqlite3_get_table(db, sql.c_str(), &re, &row, &col, NULL);
	int index = col;
	std::string strOut;
	if (SQLITE_OK == res) {
		for (int i = 0; i < row; i++) {
			std::vector<string> t;
			for (int j = 0; j < col; j++) {
				std::string d = re[index];
				t.push_back(d);
				++index;
			}
			result.push_back(t);
		}
	}
	sqlite3_free_table(re);
	return result;
}