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
ī�带 Ŭ������ �� �׸� �����ִ� �Լ�
*/
void Card::showImage() {
	SimpleAudioEngine::getInstance()->playEffect("Sounds/ding.wav", false);
	this->setTexture(Card::cardImg.at(this->getTag() - 1));

	this->schedule(schedule_selector(Card::update), 1.f);
}

/*
ī�尡 �ٽ� �ʱ� �̹����� ���ư��� �Լ�
(ī�尡 ���� ����ġ�� ���, �ι�° ī�� �Ȱ�� �ð� �ʰ� �� ��� �ΰ���)
*/
void Card::showNoImage() {
	this->setTexture("Images/box_normal.png");

	if (HelloWorld::clickedCards.size() < 2) { // ī�� �ð� �ʰ� 
		HelloWorld::clickedCards.clear();
		HelloWorld::touchFlag = true;
	}
	this->unschedule(schedule_selector(Card::update));
	
}

/*
���õ� �ΰ��� ī�尡 ��ġ�� �� ����(setColor(black))
*/
void Card::remove() {
	SimpleAudioEngine::getInstance()->playEffect("Sounds/del_block.wav", false);
	this->setColor(Color3B::BLACK);
	this->unschedule(schedule_selector(Card::update));
}

/*
ī�� �ϳ� �����ϰ� 3�� ���� ������ ���� �� �ʱ� �̹����� ���ư��� �Լ�
*/
void Card::update(float time) {
	checkTime = checkTime + time;
	if (checkTime >= 3.f) {
		if (HelloWorld::clickedCards.size() == 1) { // �ð� �ʰ�
			HelloWorld::touchFlag = false;
			this->showNoImage();
		}
		else {
			this->unschedule(schedule_selector(Card::update));
		}

		checkTime = 0.f;
	}
}
