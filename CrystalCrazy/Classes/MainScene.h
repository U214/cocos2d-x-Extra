#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);

	void update(float t);
	void pressPlay(Ref* pSender);
	void pressAbout(Ref* pSender);

	cocos2d::Vector<cocos2d::Sprite*> fallingGems;
};

#endif // __MAIN_SCENE_H__
