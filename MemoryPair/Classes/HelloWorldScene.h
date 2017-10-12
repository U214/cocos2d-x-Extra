#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Card.h"

class Card;

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	void update(float time);
	void onEnter();
	void compareCard();

	Card* card[20] = { nullptr };

	static cocos2d::Vector<Card*> clickedCards;

	static bool touchFlag;

	// 이전에 선택된 카드의 태그 값
	int preClickedCard = -1;
};

#endif // __HELLOWORLD_SCENE_H__