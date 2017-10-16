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

	void createWoman();
	void update(float time);
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);

	cocos2d::Size							winSize;
	cocos2d::Vec2							womanPosition;
	cocos2d::Sprite*						pWoman;
	cocos2d::Vector<cocos2d::TMXTiledMap*>	tmap;
	cocos2d::Vector<cocos2d::TMXLayer*>		background;
	cocos2d::Vector<cocos2d::TMXLayer*>		metainfo;

	// 배경(타일맵) 너비 값
	float tmapWidth;
	// 낭떠러지 카운트
	int cliffCnt = 1;
	// 배경(타일맵) 반복 카운트
	int roopCnt = 0;
	// 레이어 인덱스
	int idx = 0;
};

#endif // __HELLOWORLD_SCENE_H__