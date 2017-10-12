#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>
#include <ctime>

USING_NS_CC;
using namespace CocosDenshion;

Vector<Card*> HelloWorld::clickedCards;
bool HelloWorld::touchFlag = true;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}

	/////////////////////////////////
	Size size = this->getContentSize();

	for (int i = 0; i < 20; i++) {
		int col = i % 4;
		int row = i / 4;

		card[i] = Card::create();
		card[i]->setTexture("Images/box_normal.png");
		card[i]->setPosition(
			Vec2(80 * (0.6 + col * 1.05), size.height - (80 * (0.6 + row * 1.05))));
		this->addChild(card[i]);
	}

	// 카드 랜덤 배치
	srand((int)time(NULL));
	int randIdx1 = 0, randIdx2 = 0;
	int cnt = 1;

	while (cnt <= 10) {
		do {
			randIdx1 = rand() % 20;
			randIdx2 = rand() % 20;
		} while ((randIdx1 == randIdx2)
			|| (card[randIdx1]->getTag() != -1)
			|| (card[randIdx2]->getTag() != -1));

		card[randIdx1]->setTag(cnt);
		card[randIdx2]->setTag(cnt);

		cnt++;
	}

	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/ding.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/box_normal.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/del_block.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/result.wav");

	schedule(schedule_selector(HelloWorld::update));

	return true;
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		if (touchFlag) {
			if (clickedCards.size() < 2) {
				for (int i = 0; i < 20; i++) {
					if (card[i]->getBoundingBox().containsPoint(touch->getLocation())) {
						if ((preClickedCard != i) && (card[i]->getColor() != Color3B::BLACK)) {
							card[i]->showImage();
							clickedCards.pushBack(card[i]);
							preClickedCard = i;
						}
					}
				}
			}

			if (clickedCards.size() == 2) {
				touchFlag = false;

				this->runAction(Sequence::create(
					DelayTime::create(1.f),
					CallFunc::create(CC_CALLBACK_0(HelloWorld::compareCard, this)),
					NULL));
			}
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/*
선택 된 두개의 카드를 비교하는 함수
*/
void HelloWorld::compareCard() {
	if (clickedCards.at(0)->getTag() == clickedCards.at(1)->getTag()) {
		clickedCards.at(0)->remove();
		clickedCards.at(1)->remove();
	}
	else {
		clickedCards.at(0)->showNoImage();
		clickedCards.at(1)->showNoImage();
	}

	clickedCards.clear();
	touchFlag = true;
}

/*
모든 카드가 제거되면 게임 클리어
*/
void HelloWorld::update(float time) {
	for (int i = 0; i < 20; i++) {
		if (card[i]->getColor() != Color3B::BLACK) {
			break;
		}
		if ((i == 19) && (card[i]->getColor() == Color3B::BLACK)) {
			SimpleAudioEngine::getInstance()->playEffect("Sounds/result.wav", false);
			this->unschedule(schedule_selector(HelloWorld::update));
		}
	}
}