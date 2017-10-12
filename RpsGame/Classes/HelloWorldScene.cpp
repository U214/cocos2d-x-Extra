#include "HelloWorldScene.h"
#include <stdio.h>
#include <cstdlib>
#include <ctime>

USING_NS_CC;

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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	//////////////// 레이블 만들기 /////////////////
	pUserScore = LabelTTF::create("Your Score : 0", "Arial", 16);
	pComScore = LabelTTF::create("Computer Score : 0", "Arial", 16);
	pResult = LabelTTF::create("", "Arial", 22);

	pUserScore->setColor(Color3B::BLUE);
	pComScore->setColor(Color3B::BLUE);
	pResult->setColor(Color3B::RED);

	pUserScore->setPosition(Vec2(120, 300));
	pComScore->setPosition(Vec2(360, 300));
	pResult->setPosition(Vec2(240, 250));

	this->addChild(pUserScore);
	this->addChild(pComScore);
	this->addChild(pResult);

	//////////////// 메뉴 만들기 /////////////////
	auto pMenuItem1 = MenuItemFont::create(
		"[ 가위 ]",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	auto pMenuItem2 = MenuItemFont::create(
		"[ 바위 ]",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	auto pMenuItem3 = MenuItemFont::create(
		"[ 보 ]",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	auto pMenuItem4 = MenuItemFont::create(
		"[ 다시하기 ]",
		CC_CALLBACK_1(HelloWorld::reset, this));
	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, pMenuItem4, NULL);

	pMenuItem1->setTag(1);
	pMenuItem2->setTag(2);
	pMenuItem3->setTag(3);

	pMenuItem1->setColor(Color3B::BLACK);
	pMenuItem2->setColor(Color3B::BLACK);
	pMenuItem3->setColor(Color3B::BLACK);
	pMenuItem4->setColor(Color3B::BLACK);

	pMenu->setPosition(Vec2::ZERO);
	pMenuItem1->setPosition(Vec2(120, 80));
	pMenuItem2->setPosition(Vec2(240, 80));
	pMenuItem3->setPosition(Vec2(360, 80));
	pMenuItem4->setPosition(Vec2(240, 50));

	this->addChild(pMenu);

	srand((int)time(NULL));

	return true;
}

/*
가위 바위 보 버튼 클릭 후 결과 출력해주는 함수
태그 - 1. 가위 2. 바위 3. 보 11. 가위바위보 그림
*/
void HelloWorld::doClick(Ref* pSender) {
	auto tItem = (MenuItem *)pSender;
	char buf[20] = { 0 };
	int nUserValue = tItem->getTag();
	int nComValue = rand() % 3 + 1;

	int n = nUserValue - nComValue;

	if (n == 0) {
		pResult->setString("비겼습니다.");
	}
	else if ((n == -1) || (n == 2)) {
		pResult->setString("컴퓨터가 이겼습니다.");
		nComScore++;

		sprintf(buf, "Computer Score : %d", nComScore);
		pComScore->setString(buf);
	}
	else {
		pResult->setString("사용자가 이겼습니다.");
		nUserScore++;

		sprintf(buf, "Your Score : %d", nUserScore);
		pUserScore->setString(buf);
	}

	//////////////// 가위바위보 결과 그림 만들기 /////////////////
	while (this->getChildByTag(11) != NULL) { // 이전 결과 그림 삭제
		this->removeChildByTag(11, true);
	}

	sprintf(buf, "Images/img%d.png", nUserValue);
	auto pUserSprite = Sprite::create(buf);

	pUserSprite->setFlippedX(true);

	sprintf(buf, "Images/img%d.png", nComValue);
	auto pComSprite = Sprite::create(buf);

	pUserSprite->setPosition(Vec2(120, 160));
	pComSprite->setPosition(Vec2(360, 160));

	pUserSprite->setTag(11);
	pComSprite->setTag(11);

	this->addChild(pUserSprite);
	this->addChild(pComSprite);
}

/*
다시 하기 버튼 클릭 후 게임을 초기화 시켜주는 함수
*/
void HelloWorld::reset(Ref* pSender) {
	while (this->getChildByTag(11) != NULL) { // 이전 결과 그림 삭제
		this->removeChildByTag(11, true);
	}

	nUserScore = 0;
	nComScore = 0;

	pResult->setString("");
	pUserScore->setString("Your Score : 0");
	pComScore->setString("Computer Score : 0");
}