#include "AboutScene.h"

USING_NS_CC;

Scene* AboutScene::createScene()
{
	return AboutScene::create();
}

bool AboutScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////////


	return true;
}
