#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(HelloWorld);


    cocos2d::Sprite* pMole1;
    cocos2d::Sprite* pMole2;
    cocos2d::Sprite* pMole3;

    void showMole(float f);
    void popMole(cocos2d::Sprite* mole);
};

#endif // __HELLOWORLD_SCENE_H__
