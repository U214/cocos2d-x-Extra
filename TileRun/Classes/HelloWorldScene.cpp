#include "HelloWorldScene.h"

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

	this->addChild(LayerColor::create(Color4B(255, 255, 255, 255)));

	/////////////////////////////////
	winSize = Director::getInstance()->getWinSize();

	// 배경 레이어가 최소 2개가 있어야 배경 반복 가능
	while (tmap.size() < 2) {
		tmap.pushBack(TMXTiledMap::create("TileMaps/TestForest.tmx"));
		background.pushBack(tmap.back()->getLayer("Background"));
		metainfo.pushBack(tmap.back()->getLayer("MetaInfo"));
		metainfo.back()->setVisible(false);
		this->addChild(tmap.back(), 1);
	}

	tmapWidth = tmap.back()->getMapSize().width * tmap.back()->getTileSize().width;

	// 같은 배경 레이어를 나란히 붙히기 위해 두번째 레이어 위치 설정
	tmap.back()->setPosition(Vec2(tmapWidth, 0));

	TMXObjectGroup* objects = tmap.at(0)->getObjectGroup("Objects");
	ValueMap& spawnPoint = objects->getObject("SpawnPoint");

	womanPosition = Vec2(spawnPoint["x"].asInt(), spawnPoint["y"].asInt());
	this->createWoman();

	tmap.at(0)->runAction(RepeatForever::create(MoveBy::create(1, Vec2(-100, 0))));
	tmap.at(1)->runAction(RepeatForever::create(MoveBy::create(1, Vec2(-100, 0))));

	schedule(schedule_selector(HelloWorld::update));

	return true;
}

void HelloWorld::createWoman() {
	auto texture = Director::getInstance()->getTextureCache()
		->addImage("Images/grossinis_sister1.png");

	pWoman = Sprite::createWithTexture(texture, Rect(0, 0, 52, 120));
	pWoman->setPosition(womanPosition);
	this->addChild(pWoman);

	pWoman->setScale(0.5f);
}

void HelloWorld::update(float time) {
	Vec2 womanTileCoord = this->tileCoordForPosition(womanPosition);
	Vec2 layerTileCoord = this->tileCoordForPosition(tmap.at(idx)->getPosition());

	// 계속 이동하는 레이어에서 woman의 위치를 구함
	int tileGid = this->metainfo.at(idx)->getTileGIDAt(
		Vec2(womanTileCoord.x - layerTileCoord.x, womanTileCoord.y));

	if (!tileGid) {
		log("낭떠러지예요~ %d", cliffCnt);
		cliffCnt++;
	}

	// 레이어의 끝에 도달하면 다른 레이어로 실행
	if ((womanTileCoord.x - layerTileCoord.x)
		== (tmap.at(idx)->getMapSize().width - 1)) {
		idx = (idx == 0) ? 1 : 0;

		roopCnt++;
	}

	// 레이어의 4분의 1에 도달했을 경우 왼쪽의 다른 레이어를 현재 레이어 끝에 붙힌다
	// 단, 이전 레이어가 없는 첫번째 레이어의 경우 실행하지 않음
	if (roopCnt > 0) {
		if ((womanTileCoord.x - layerTileCoord.x)
			== (tmap.at(idx)->getMapSize().width / 4)) {
			int preIdx = (idx == 0) ? 1 : 0;
			tmap.at(preIdx)->setPosition(
				Vec2(tmap.at(idx)->getPosition().x + tmapWidth, 0));
		}
	}
}

Vec2 HelloWorld::tileCoordForPosition(Vec2 position) {
	int x = position.x / tmap.back()->getTileSize().width;
	int y = ((tmap.back()->getMapSize().height * tmap.back()->getTileSize().height) - position.y)
		/ tmap.back()->getTileSize().height;

	return Vec2(x, y);
}
