#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

    void doClick(Ref* pSender);
    void doShowScore();
    void doReset(Ref* pSender);

    int nUserScore;
    int nComScore;
    cocos2d::LabelTTF* pYourScore;
    cocos2d::LabelTTF* pComScore;
    cocos2d::LabelTTF* pResult;
};

#endif // __HELLOWORLD_SCENE_H__
