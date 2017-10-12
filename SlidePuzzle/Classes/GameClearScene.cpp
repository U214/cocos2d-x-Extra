#include "GameClearScene.h"

USING_NS_CC;

Scene* GameClearScene::createScene()
{
	return GameClearScene::create();
}

bool GameClearScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	/////////////////////////////////
	auto texture =
		Director::getInstance()->getTextureCache()->addImage("image.png");
	auto pSprite = Sprite::createWithTexture(
		texture,
		Rect(0, 0, 540, 540));

	pSprite->setPosition(Vec2(
		this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(pSprite);

	return true;
}