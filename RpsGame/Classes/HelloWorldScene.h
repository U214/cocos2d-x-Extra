#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
	int nUserScore = 0;
	int nComScore = 0;
	cocos2d::LabelTTF* pUserScore = NULL;
	cocos2d::LabelTTF* pComScore = NULL;
	cocos2d::LabelTTF* pResult = NULL;

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);
	void doClick(Ref* pSender);
	void reset(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__