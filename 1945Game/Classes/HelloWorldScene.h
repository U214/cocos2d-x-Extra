#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "AirPlane.h"
#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

	void onEnter();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	void Update(float time);
	// 플레이어 생성
	void CreatePlayer(float time);
	// 적 비행기 리젠
	void RegenEnemy(float time);
	// 비행기 미사일 발사
	void Shooting(float time);

	static Scene* scene_;
	static Size size_;

private:
	AirPlane* player_;
	Vector<AirPlane*> enemy_;

	LabelTTF* pScore;
	LabelTTF* pLife;
	Vec2 distance_;
	float regenCheckTime_ = -1.f;
	int life = 3;
	int score = 0;
};

#endif // __HELLOWORLD_SCENE_H__
