#ifndef __GAME_SCENE_H_
#define __GAME_SCENE_H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

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

	bool isGameOver = false;
	bool isDisplayingHint = false;
	int score = 0;

	cocos2d::Vector<cocos2d::Sprite*> board;
	cocos2d::Vector<cocos2d::Sprite*> gemsInColumn;

};

#endif // __GAME_SCENE_H_