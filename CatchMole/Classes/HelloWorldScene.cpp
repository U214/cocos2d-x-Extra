#include "HelloWorldScene.h"
#include <ctime>
#include <cstdlib>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
	/////////////////////////////////
	srand((int)time(NULL));

	auto pGrassUpper = Sprite::create("Images/grass_upper.png");
	auto pGrassLower = Sprite::create("Images/grass_lower.png");
	pMole[0] = Sprite::create("Images/mole_1.png");
	pMole[1] = Sprite::create("Images/mole_1.png");
	pMole[2] = Sprite::create("Images/mole_1.png");

	pGrassUpper->setPosition(Vec2(480, 320));
	pGrassUpper->setAnchorPoint(Vec2(1, 1));

	pGrassLower->setPosition(Vec2(0, 1));
	pGrassLower->setAnchorPoint(Vec2(0, 0));
	
	pMole[0]->setPosition(Vec2(96, 90));
	pMole[1]->setPosition(Vec2(242, 90));
	pMole[2]->setPosition(Vec2(386, 90));
	
	this->addChild(pGrassUpper, 1);
	this->addChild(pGrassLower, 3);
	this->addChild(pMole[0], 2);
	this->addChild(pMole[1], 2);
	this->addChild(pMole[2], 2);

	pMole[0]->runAction(
		RepeatForever::create(
			Sequence::create(
				CallFunc::create(CC_CALLBACK_0(HelloWorld::randMole, this, 0)),
				DelayTime::create(3.25),
				NULL)));

	pMole[1]->runAction(
		RepeatForever::create(
			Sequence::create(
				CallFunc::create(CC_CALLBACK_0(HelloWorld::randMole, this, 1)),
				DelayTime::create(3.25),
				NULL)));

	pMole[2]->runAction(
		RepeatForever::create(
			Sequence::create(
				CallFunc::create(CC_CALLBACK_0(HelloWorld::randMole, this, 2)),
				DelayTime::create(3.25),
				NULL)));

    return true;
}

void HelloWorld::randMole(int moleNum) {
	float delayNum = rand() % 2 + 0.2;
	int upDownFlag = rand() % 4;

	if (upDownFlag == 0) {
		pMole[moleNum]->runAction(DelayTime::create(1));
	}
	else {
		pMole[moleNum]->runAction(Sequence::create(
			MoveBy::create(0.15, Vec2(0, 80)),
			DelayTime::create(delayNum),
			MoveBy::create(0.1, Vec2(0, -80)),
			NULL));
	}
}