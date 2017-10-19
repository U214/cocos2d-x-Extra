#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////////
	// 배경 이미지 추가
	auto pBackgroud = Sprite::create("gamescene/resources-auto/background.png");
	pBackgroud->setPosition(Vec2(320, 480));
	this->addChild(pBackgroud);

	setupBoard();

	// 타이머 구현 자리

	//this->schedule(schedule_selector(GameScene::update));

	return true;
}

void GameScene::update(float t) {

}

void GameScene::setupBoard() {
	char str[50] = { 0 };

	for (int i = 0; i < TOTAL_NUM; i++) {
		int x = i % WIDTH_NUM;
		int y = i / WIDTH_NUM;

		int randNum = rand() % 5;
		sprintf(str, "crystals/resources-auto/%d.png", randNum);

		auto pGemSprite = Sprite::create(str);
		pGemSprite->setTag(randNum);
		pGemSprite->setScale(0.8f);
		pGemSprite->setAnchorPoint(Vec2::ZERO);
		pGemSprite->setPosition(Vec2(80 * x, (this->getContentSize().height + 120) * y));
		log("%f. %f", pGemSprite->getPosition().x, pGemSprite->getPosition().y);
		this->addChild(pGemSprite);
		pGemSprite->runAction(MoveTo::create(1.3f,
			Vec2(pGemSprite->getPosition().x, 80 * y)));

		board.pushBack(pGemSprite);
	}
}

void GameScene::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}