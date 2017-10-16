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

	// ��� ���̾ �ּ� 2���� �־�� ��� �ݺ� ����
	while (tmap.size() < 2) {
		tmap.pushBack(TMXTiledMap::create("TileMaps/TestForest.tmx"));
		background.pushBack(tmap.back()->getLayer("Background"));
		metainfo.pushBack(tmap.back()->getLayer("MetaInfo"));
		metainfo.back()->setVisible(false);
		this->addChild(tmap.back(), 1);
	}

	tmapWidth = tmap.back()->getMapSize().width * tmap.back()->getTileSize().width;

	// ���� ��� ���̾ ������ ������ ���� �ι�° ���̾� ��ġ ����
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

	// ��� �̵��ϴ� ���̾�� woman�� ��ġ�� ����
	int tileGid = this->metainfo.at(idx)->getTileGIDAt(
		Vec2(womanTileCoord.x - layerTileCoord.x, womanTileCoord.y));

	if (!tileGid) {
		log("������������~ %d", cliffCnt);
		cliffCnt++;
	}

	// ���̾��� ���� �����ϸ� �ٸ� ���̾�� ����
	if ((womanTileCoord.x - layerTileCoord.x)
		== (tmap.at(idx)->getMapSize().width - 1)) {
		idx = (idx == 0) ? 1 : 0;

		roopCnt++;
	}

	// ���̾��� 4���� 1�� �������� ��� ������ �ٸ� ���̾ ���� ���̾� ���� ������
	// ��, ���� ���̾ ���� ù��° ���̾��� ��� �������� ����
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
