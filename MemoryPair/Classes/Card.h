#ifndef __CARD_H__
#define __CARD_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "HelloWorldScene.h"
#include "cocos2d.h"

class HelloWorld;

class Card : public cocos2d::Sprite
{
public:
	static Card* create();
	void showImage();
	void showNoImage();
	void remove();
	void update(float time);

private:
	float checkTime = 0.f;

	static cocos2d::Vector<cocos2d::Texture2D*> cardImg;
};

#endif // __CARD_H__