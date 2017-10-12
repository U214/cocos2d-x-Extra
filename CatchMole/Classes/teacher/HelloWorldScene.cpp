#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    /////////////////////////////

    Sprite* pGrassLower = Sprite::create("Images/grass_lower.png");
    pGrassLower->setPosition(Vec2(240, 80));
    pGrassLower->getTexture()->setAliasTexParameters();
    this->addChild(pGrassLower, 10);

    Sprite* pGrassUpper = Sprite::create("Images/grass_upper.png");
    pGrassUpper->setPosition(Vec2(240, 240 - 1));
    pGrassUpper->getTexture()->setAliasTexParameters();
    this->addChild(pGrassUpper);



    pMole1 = Sprite::create("Images/mole_1.png");
    pMole1->setPosition(Vec2(95, 95));
    pMole1->setTag(1);
    this->addChild(pMole1);

    pMole2 = Sprite::create("Images/mole_1.png");
    pMole2->setPosition(Vec2(240, 95));
    pMole2->setTag(2);
    this->addChild(pMole2);

    pMole3 = Sprite::create("Images/mole_1.png");
    pMole3->setPosition(Vec2(385, 95));
    pMole3->setTag(3);
    this->addChild(pMole3);

    srand((int)time(nullptr));
    this->schedule(schedule_selector(HelloWorld::showMole), 0.5f);

    return true;
}

void HelloWorld::showMole(float f)
{

    for (int i = 0; i<3; i++) {

        if (rand() % 3 == 0) {
            if (i == 0) {
                if (pMole1->getNumberOfRunningActions() == 0)
                {
                    this->popMole(pMole1);
                }
            }
            else if (i == 1) {
                if (pMole2->getNumberOfRunningActions() == 0)
                {
                    this->popMole(pMole2);
                }
            }
            else {
                if (pMole3->getNumberOfRunningActions() == 0)
                {
                    this->popMole(pMole3);
                }
            }

        }
    }
}

void HelloWorld::popMole(Sprite* mole)
{
    auto moveUp = MoveBy::create(0.2f, Vec2(0, 75));
    auto easeMoveUp = EaseInOut::create(moveUp, 3.0f);
    auto easeMoveDown = easeMoveUp->reverse();
    auto delay = DelayTime::create(0.5);

    auto action = Sequence::create(easeMoveUp,
                                   delay,
                                   easeMoveDown,
                                   nullptr);
    mole->runAction(action);
}



