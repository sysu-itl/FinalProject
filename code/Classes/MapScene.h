#pragma once

#include <string>
#include "cocos2d.h"
#include "map.h"

USING_NS_CC;
using namespace std;

enum DrawStates {
	DRAW_NONE,
	DRAW_READY,
	DRAW_ACTIVE,
	DRAW_SELECT,
	DRAW_MOVE,
};

class MapScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//绘制背景
	void drawBackground();

	//菜单
	void addMenu();

	//绘制基本地图元素
	void drawMapElement();

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
	
private:

	//是否在绘制区域
	bool MapScene::isInDrawLocal(const Point& loc);

	//添加监听
	void addListener();
	
	//鼠标监听
	bool onTouchBegan(Touch* touch, Event* event);
	bool onTouchMoved(Touch* touch, Event* event);
	bool onTouchEnded(Touch* touch, Event* event);

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

