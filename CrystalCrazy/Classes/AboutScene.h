#ifndef __ABOUT_SCENE_H_
#define __ABOUT_SCENE_H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class AboutScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(AboutScene);
};

#endif // __ABOUT_SCENE_H_