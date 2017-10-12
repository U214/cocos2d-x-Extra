#include "HelloWorldScene.h"

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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);
    
	//////////////////////////////////////

	texture =
		Director::getInstance()->getTextureCache()->addImage("Images/player2.png");

	doRunRepForAction();

	schedule(schedule_selector(HelloWorld::update));

    return true;
}

void HelloWorld::update(float time) {
	if (pMan->getNumberOfRunningActionsByTag(2) > 2) {
		touchFlag = false;
		pMan->stopActionByTag(2);
	}
	else if (pMan->getNumberOfRunningActionsByTag(2) == 0) {
		touchFlag = true;
	}
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (touchFlag) {
		pMan->stopActionByTag(1);

		auto jumpSeq = Sequence::create(
			EaseOut::create(JumpTo::create(0.7f, Vec2(pMan->getPositionX(), 160), 50, 1), 0.7f),
			CallFunc::create(CC_CALLBACK_0(HelloWorld::doRunRepForAction, this)),
			NULL);
		jumpSeq->setTag(2);

		pMan->runAction(jumpSeq);
	}
	return true;
}

void HelloWorld::doRunRepForAction() {
	this->removeChildByTag(3);

	pMan = Sprite::create("Images/player2.png",
		Rect(0, 0, 30, 30));
	pMan->setScale(1.5f);
	pMan->setPosition(Vec2(240, 160));
	pMan->setTag(3);
	this->addChild(pMan);

	animation = Animation::create();
	animation->setDelayPerUnit(0.05f);

	for (int i = 0; i < 16; i++) {
		animation->addSpriteFrameWithTexture(
			texture,
			Rect(i * 30, 0, 30, 30));
	}

	rep = RepeatForever::create(Animate::create(animation));
	rep->setTag(1);
	pMan->runAction(rep);
}