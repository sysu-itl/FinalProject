#pragma once

#include <string>
#include "cocos2d.h"
#include "map.h"
#include "GameShared.h"

USING_NS_CC;
using namespace std;

enum DrawStates {
	DRAW_NONE,
	DRAW_READY,
	DRAW_ACTIVE,
	DRAW_MENU,
	DRAW_MOVE,
	DRAW_TEST,
};

class MapScene : public  cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);
	static cocos2d::Scene* createScene();

	virtual bool init();

	//temp
	void print(string str);
	Label* printLabel;


	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);
private:

	Vec2 origin;
	Size visibleSize;

	//绘制状态
	DrawStates state;
	DrawStates lastState;
	Point drawStart;

	//选取地图单元
	Sprite* selSprite;

	//当前绘制元素
	Sprite* drawSprite;

	//地图绘制单元
	vector<Sprite*> mapElement;

	// 地图实例
	MapClass map;

	//背景
	TMXTiledMap* background;

	//菜单
	Sprite* backphoto;
	Label* saveLabel;
	Label* loadLabel;
	Label* testLabel;
	Label* backLabel;
	Label* clearLabel;

	//测试
	PhysicsWorld * m_physicWorld;

	Sprite* player;
	int canJump;
	int jumpCount;
	bool gameover;

private:

	//绘制背景
	void drawBackground();

	//菜单
	void addMenu();

	//清空地图
	void clearMap();

	//绘制基本地图元素
	void drawMapElement();

	//是否在绘制区域
	bool MapScene::isInDrawLocal(const Point& loc);

	//显示和隐藏菜单
	void showMenu();
	void hideMenu();

	//移动窗口
	void moveWin(const Point& translation);

	//测试地图
	void testMenu();

	//测试初始化
	void testInit();

	//测试更新运行
	void update(float dt);

	//测试停止
	void StopAction(float dt);

	//player创建
	void createPlayer();

	//测试重置
	void testReset();

	//返回主页
	void backMainPage();

	//添加监听
	void addListener();

	//精灵碰撞
	bool onContactBegin(PhysicsContact & contact);
	bool pairMatch(int a1, int a2, int b1, int b2);

	//鼠标监听
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	//地图加载和保存
	void loadMapButtonCallback();
	void saveMapButtonCallback();

	//键盘监听
	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);

	// 坐标转换
	static inline Point localConvert(const Point& loc) {
		int w = int(loc.x) / MapScene::board;
		int h = int(loc.y) / MapScene::board;
		return Point(w*MapScene::board, h*MapScene::board);
	}
	static const int board = 32;
};
