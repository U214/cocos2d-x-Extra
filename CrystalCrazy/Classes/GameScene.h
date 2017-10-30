#ifndef __GAME_SCENE_H_
#define __GAME_SCENE_H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include <array>

#define WIDTH_NUM 8
#define HEIGHT_NUM 10
#define TOTAL_NUM 80

class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	void onEnter();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void setupBoard();
	void update(float t);
	void clock(float t);
	void doRemoveFromParent(Ref* pSender);

	bool createGem();
	void testCreateGem();
	void removeConnectedGems(int x, int y, int gemType);
	void findConnectedGems(int x, int y, int gemType);
	void endAction(int col, int j);
	void displayHint();
	void activatePowerUp(int x, int y, int hStart, int vStart);

	void updateSparkle();
	//void createGameOver();

	bool isGameOver = false;
	bool isDisplayingHint = false;
	bool isCreated = true;
	bool isRemoved = false;
	bool isChanging = false;
	bool isFinding = false;

	cocos2d::Sprite*			idxPup;
	cocos2d::ProgressTimer*     timer;
	cocos2d::BlendFunc			blend;

	int score = 0;
	int lastScore = 0;
	float lastMoveTime = 0.f;
	float timeLeft = 0.f;

	std::array<cocos2d::Sprite*, 80> board;
	cocos2d::Vector<cocos2d::Sprite*> connectedGems;

};

#endif // __GAME_SCENE_H_