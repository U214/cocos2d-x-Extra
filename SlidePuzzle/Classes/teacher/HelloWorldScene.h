#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Tile.h"

#define NUM_HORIZONTAL_PIECES   3
#define NUM_VERTICAL_PIECES     3
#define SPACE_PIECES            1


using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

    
    Size winSize;
    Vector<Sprite*> cards;      // 바뀌는 위치들 기록 배열
    Vector<Sprite*> cardspoint; // 원본 위치들 기록 배열
    Texture2D* pTexture;        // 원본 그림
    
    bool bTouch;                // 현재 카드체크 진행중인지 여부
    Vec2 blankPoint;            // 비어 있는 곳의 좌표

    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    void initGame();
    void moveCard(cocos2d::Vec2 touchPoint);
    void checkComplete();

};

#endif // __HELLOWORLD_SCENE_H__
