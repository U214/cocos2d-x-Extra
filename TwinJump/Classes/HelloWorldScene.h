﻿#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

	cocos2d::Sprite* pMan;
	cocos2d::RepeatForever* rep;
	cocos2d::Animation* animation;
	cocos2d::Texture2D* texture;

	void update(float time);
	void onEnter();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void doRunRepForAction();

	bool touchFlag = true;
};

#endif // __HELLOWORLD_SCENE_H__
