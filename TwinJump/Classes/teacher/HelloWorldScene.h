#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

    cocos2d::Sprite* pMan;
    float yValue;
    int nJumpCount;
	bool bDown;
    
    virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void doJump();
    void myTick(float f);
};

#endif // __HELLOWORLD_SCENE_H__
