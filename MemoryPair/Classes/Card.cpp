#include "Card.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Vector<Texture2D*> Card::cardImg;


Card* Card::create() {
	Card* card = new Card();

	if (card && card->initWithTexture(nullptr, Rect::ZERO)) {
		card->autorelease();

		if (Card::cardImg.size() == 0) {
			char buf[20] = { 0 };
			for (int i = 1; i <= 10; i++) {
				;
				sprintf(buf, "Images/card%02d.png", i);
				Card::cardImg.pushBack(Director::getInstance()->getTextureCache()->addImage(buf));
			}
		}
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}

/*
카드를 클릭했을 때 그림 보여주는 함수
*/
void Card::showImage() {
	SimpleAudioEngine::getInstance()->playEffect("Sounds/ding.wav", false);
	this->setTexture(Card::cardImg.at(this->getTag() - 1));

	this->schedule(schedule_selector(Card::update), 1.f);
}

/*
카드가 다시 초기 이미지로 돌아가는 함수
(카드가 서로 불일치할 경우, 두번째 카드 안골라서 시간 초과 될 경우 두가지)
*/
void Card::showNoImage() {
	this->setTexture("Images/box_normal.png");

	if (HelloWorld::clickedCards.size() < 2) { // 카드 시간 초과 
		HelloWorld::clickedCards.clear();
		HelloWorld::touchFlag = true;
	}
	this->unschedule(schedule_selector(Card::update));
	
}

/*
선택된 두개의 카드가 일치할 때 제거(setColor(black))
*/
void Card::remove() {
	SimpleAudioEngine::getInstance()->playEffect("Sounds/del_block.wav", false);
	this->setColor(Color3B::BLACK);
	this->unschedule(schedule_selector(Card::update));
}

/*
카드 하나 선택하고 3초 동안 반응이 없을 때 초기 이미지로 돌아가는 함수
*/
void Card::update(float time) {
	checkTime = checkTime + time;
	if (checkTime >= 3.f) {
		if (HelloWorld::clickedCards.size() == 1) { // 시간 초과
			HelloWorld::touchFlag = false;
			this->showNoImage();
		}
		else {
			this->unschedule(schedule_selector(Card::update));
		}

		checkTime = 0.f;
	}
}
