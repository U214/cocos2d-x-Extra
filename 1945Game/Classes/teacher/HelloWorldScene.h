#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#define MAX_MISSILE		2

USING_NS_CC;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

	void menuCloseCallback(Ref* pSender);

	void onEnter();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);

	void Update(float time);
	void RegenEnemy(float time);
	void Shooting(float time);

	void PutCrashEffect(const Vec2 &pos);
	void AddEnemy(const Vec2& position);

	SimpleAudioEngine* simpleAudio;
    void explosionSound(float time);
    void shipDestroySound(float time);
    void fireSound(float time);

private:
	Sprite *player_;//주인공 플레이어
	int playerLife;

	// 점수 및 점수판
	int score;
	LabelTTF* scoreLabel;
	//Array *missile_[MAX_MISSILE];
	//Array *enemy_;//적비행기 CCArray(배열, 리스트 구조)
	
	bool stopGame;

	Vector<Sprite *> enemy_;
	Vector<Sprite*> missile_;

	Vec2 distance_;
	Size size_;

	float regenCheckTime_;
};


#endif // __HELLOWORLD_SCENE_H__
