#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "AstarItem.h"

USING_NS_CC;
using namespace std;

#define MAX_X   7
#define MAX_Y   7
#define SIZE   32.0f
#define   MOVE   32

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	Size winSize;
	LayerColor* board;
	char arrCanvas[MAX_X][MAX_Y];
	Sprite* tamara;

	void drawLines();
	void drawMap();

	bool onTouchBegan(Touch* touch, Event* event);

	// Astar Start ///////////////////
	Vec2 pCoordS;
	Vec2 pCoordE;

	vector<AstarItem*>* openPath;
	vector<AstarItem*>* closePath;
	vector<Vec2>* myPath;

	void AStartPathFind();
	void findPath();
	void movePath();
	void isRoad(Vec2 mPos, Vec2 pPos, int pGCost, int mGCost);
	Vec2 CoordForPosition(Vec2 position);
	int getDistance(Vec2 d1, Vec2 d2, int type);
	// Astar End ////////////////////////
};

#endif // __HELLOWORLD_SCENE_H__