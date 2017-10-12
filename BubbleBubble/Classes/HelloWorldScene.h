#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	bool vibarFlag = true;

	void doClick(Ref* pSender);
	cocos2d::Sequence* actionFadeInOut(float time, int opacity = -1);
	cocos2d::Sequence* actionMovePlace(float time, cocos2d::Vec2 movePos, cocos2d::Vec2 placePos);
};

#endif // __HELLOWORLD_SCENE_H__