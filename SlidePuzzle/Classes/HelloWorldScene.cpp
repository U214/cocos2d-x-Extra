#include "HelloWorldScene.h"
#include "GameClearScene.h"
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

	/////////////////////////////////////////////
    
	auto size = this->getContentSize();

	auto texture =
		Director::getInstance()->getTextureCache()->addImage("image.png");

	for (int i = 0; i < 9; i++) {
		int column = i % 3;
		int row = i / 3;

		pieces[i] = Sprite::createWithTexture(
			texture,
			Rect(column * 180, row * 180, 180, 180));
		
		if (i == 8) {
			pieces[i]->setColor(Color3B::BLACK);
		}

		pieces[i]->setPosition(Vec2(180 * (0.5 + column * 1.05), size.height - (180 * (0.5 + row * 1.05))));
		pieces[i]->setTag(i);
		this->addChild(pieces[i]);
	}

	int randNum = 0, mixNum = 200;
	int directionArr[4] = { 1, 2, 3, 4 };
	Vec2 pos;
	Sprite* pSprite;

	srand((int)time(NULL));

	// 초기 퍼즐 배치
	while (mixNum) {
		randNum = rand() % 4;
		moveNumByInputKey(directionArr[randNum]);
		mixNum--;
	}

	while (nullIdx % 3 != 2) {
		pSprite = pieces[nullIdx];
		pos = pieces[nullIdx]->getPosition();

		pieces[nullIdx]->setPosition(pieces[nullIdx + 1]->getPosition());
		pieces[nullIdx + 1]->setPosition(pos);

		pieces[nullIdx] = pieces[nullIdx + 1];
		pieces[nullIdx + 1] = pSprite;

		nullIdx++;
	}

	while (nullIdx != 8) {
		pSprite = pieces[nullIdx];
		pos = pieces[nullIdx]->getPosition();

		pieces[nullIdx]->setPosition(pieces[nullIdx + 3]->getPosition());
		pieces[nullIdx + 3]->setPosition(pos);

		pieces[nullIdx] = pieces[nullIdx + 3];
		pieces[nullIdx + 3] = pSprite;

		nullIdx = nullIdx + 3;
	}

	schedule(schedule_selector(HelloWorld::update));

    return true;
}

void HelloWorld::update(float time) {
	for (int i = 0; i < 9; i++) {
		if (pieces[i]->getTag() != i) {
			break;
		}
		else if (i == 8) {
			Director::getInstance()->pushScene(
				TransitionFade::create(1.f, GameClearScene::createScene()));

			unschedule(schedule_selector(HelloWorld::update));
		}
	}
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	for (int i = 0; i < 9; i++) {
		if (i == nullIdx) {
			continue;
		}

		if (pieces[i]->getBoundingBox().containsPoint(touch->getLocation())) {
			switch (nullIdx - i) 
			{
			case 3: // down
				moveNumByInputKey(4);
				break;

			case -3: // up
				moveNumByInputKey(3);
				break;

			case 1: // right
				moveNumByInputKey(1);
				break;

			case -1: // left
				moveNumByInputKey(2);
				break;
			}
		}
	}

	return true;
}

/*
void moveNumByInputKey(int direction)
입력 받은 방향값에 의해 퍼즐칸이 이동하는 함수

int direction : 입력 받은 방향 값
*/
void HelloWorld::moveNumByInputKey(int direction) {
	Vec2 pos;
	Sprite* pSprite;

	switch (direction)
	{
	case 1:	//RIGHT
		if (nullIdx % 3 != 0) {
			pSprite = pieces[nullIdx];
			pos = pieces[nullIdx]->getPosition();

			pieces[nullIdx]->setPosition(pieces[nullIdx - 1]->getPosition());
			pieces[nullIdx - 1]->setPosition(pos);

			pieces[nullIdx] = pieces[nullIdx - 1];
			pieces[nullIdx - 1] = pSprite;

			nullIdx--;
		}
		break;

	case 2:	//LEFT
		if (nullIdx % 3 != 2) {
			pSprite = pieces[nullIdx];
			pos = pieces[nullIdx]->getPosition();

			pieces[nullIdx]->setPosition(pieces[nullIdx + 1]->getPosition());
			pieces[nullIdx + 1]->setPosition(pos);

			pieces[nullIdx] = pieces[nullIdx + 1];
			pieces[nullIdx + 1] = pSprite;

			nullIdx++;
		}
		break;

	case 3:	//UP
		if (nullIdx < 6) {
			pSprite = pieces[nullIdx];
			pos = pieces[nullIdx]->getPosition();

			pieces[nullIdx]->setPosition(pieces[nullIdx + 3]->getPosition());
			pieces[nullIdx + 3]->setPosition(pos);

			pieces[nullIdx] = pieces[nullIdx + 3];
			pieces[nullIdx + 3] = pSprite;

			nullIdx = nullIdx + 3;
		}
		break;

	case 4:	//DOWN
		if (nullIdx > 2) {
			pSprite = pieces[nullIdx];
			pos = pieces[nullIdx]->getPosition();

			pieces[nullIdx]->setPosition(pieces[nullIdx - 3]->getPosition());
			pieces[nullIdx - 3]->setPosition(pos);

			pieces[nullIdx] = pieces[nullIdx - 3];
			pieces[nullIdx - 3] = pSprite;

			nullIdx = nullIdx - 3;
		}

		break;
	}
}