#include "MainScene.h"
#include "AboutScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>
#include <ctime>

USING_NS_CC;

using namespace CocosDenshion;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

bool MainScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	/////////////////////////////////
	srand((int)time(NULL));

	// 배경 이미지 추가
	auto pBackgroud = Sprite::create("gamescene/resources-auto/background.png");
	pBackgroud->setPosition(Vec2(320, 480));
	this->addChild(pBackgroud);

	// Setup particles in background
	ParticleSystem* emitter = ParticleSystemQuad::create("particles/bg-stars.plist");
	this->addChild(emitter);

	// 버튼 메뉴 추가
	auto pPlayBtn = MenuItemImage::create(
		"mainscene/resources-auto/btn-play.png",
		"mainscene/resources-auto/btn-play-down.png",
		CC_CALLBACK_1(MainScene::pressPlay, this));
	
	auto pAboutBtn = MenuItemImage::create(
		"mainscene/resources-auto/btn-about.png",
		"mainscene/resources-auto/btn-about-down.png",
		CC_CALLBACK_1(MainScene::pressAbout, this));

	auto pMenu = Menu::create(pPlayBtn, pAboutBtn, NULL);
	pMenu->alignItemsVertically();
	pMenu->setScale(0.5f);
	pMenu->setAnchorPoint(Vec2::ZERO);
	this->addChild(pMenu, 0, 100);

	this->schedule(schedule_selector(MainScene::update));

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("crystals.plist")

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/loop.mp3", true);

    return true;
}

void MainScene::update(float t) {
	if (rand() / 100 < 20) {
		float randFloat = rand() / (float)RAND_MAX;

		char str[50] = { 0 };
		sprintf(str, "crystals/resources-auto/%d.png", rand() % 5);

		auto pGemSprite = Sprite::create(str);
		pGemSprite->setScale(randFloat * 0.7);
		pGemSprite->setPosition(
			randFloat * this->getContentSize().width, this->getContentSize().height + 80);

		fallingGems.pushBack(pGemSprite);

		this->addChild(pGemSprite);
	}

	for (int i = fallingGems.size() - 1; i >= 0; i--) {
		auto gem = fallingGems.at(i);
		auto speed = 4 * gem->getScale();

		gem->setPosition(Vec2(gem->getPosition().x, gem->getPosition().y - speed));

		if (gem->getPosition().y < -40) {
			fallingGems.eraseObject(gem);
		}
	}
}

void MainScene::pressPlay(Ref* pSender) {

	SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav");
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->replaceScene(GameScene::createScene());
}

void MainScene::pressAbout(Ref* pSender) {
	SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav");

	auto pMenu = (Menu*)this->getChildByTag(100);
	pMenu->setEnabled(false);

	Director::getInstance()->pushScene(AboutScene::createScene());
}