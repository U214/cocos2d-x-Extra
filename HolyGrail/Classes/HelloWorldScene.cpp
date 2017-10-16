#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
	if ( !Scene::init() )
    {
        return false;
    }

	/////////// 기본 UI 레이어 추가 /////////////////
	auto defaultLayer = Layer::create(); 

	auto sideUI = Sprite::create("Images/SideUi.png");
	sideUI->setPosition(Vec2(460, this->getContentSize().height / 2));
	sideUI->setOpacity(150);
	defaultLayer->addChild(sideUI);

	auto sideB1 = Sprite::create("Images/SideB1.png");
	sideB1->setPosition(Vec2(
		453, this->getContentSize().height - 32));
	sideB1->setOpacity(200);
	defaultLayer->addChild(sideB1);

	texture = Director::getInstance()->getTextureCache()
		->addImage("Images/Holygrail.png");

	// 아이템 박스 투명 이미지
	itemBox = Sprite::createWithTexture(
		texture,
		Rect(texture->getContentSize().width - 46, texture->getContentSize().height - 45, 46, 45));
	itemBox->setPosition(Vec2(
		sideB1->getPosition().x, this->getContentSize().height - 32));
	defaultLayer->addChild(itemBox);

	auto sideB2Item = MenuItemImage::create(
		"Images/SideB2.png",
		"Images/SideB2.png",
		CC_CALLBACK_1(HelloWorld::initMap, this));
	sideB2Item->setPosition(Vec2(
		sideB1->getPosition().x, 30));
	sideB2Item->setOpacity(200);

	auto sideB2Menu = Menu::create(sideB2Item, NULL);
	sideB2Menu->setPosition(Vec2::ZERO);
	defaultLayer->addChild(sideB2Menu);

	stage = LabelTTF::create("", "Thonburi", 14);
	stage->setPosition(Vec2(sideB1->getPosition().x, sideB2Item->getPosition().y + 40));
	stage->setColor(Color3B::WHITE);
	defaultLayer->addChild(stage);

	this->addChild(defaultLayer, 1);

	/////////////////////////////////////////////

	stageColor.push_back(Color4B(17, 146, 8, 255));
	stageColor.push_back(Color4B(17, 146, 8, 255));
	stageColor.push_back(Color4B(184, 184, 184, 255));

	objectMap.insert({ "sword", "wolf" });
	objectMap.insert({ "mace", "jelly" });
	objectMap.insert({ "holySword", "demon" });
	objectMap.insert({ "key", "box" });

	itemFrameMap.insert({ "sword", "W_Sword001.png" });
	itemFrameMap.insert({ "mace", "W_Mace008.png" });
	itemFrameMap.insert({ "holySword", "W_Sword015.png" });
	itemFrameMap.insert({ "key", "I_Key02.png" });

	objectTagMap.insert({ "sword", 100 });
	objectTagMap.insert({ "mace", 200 });
	objectTagMap.insert({ "holySword", 300 });
	objectTagMap.insert({ "wolf", 400 });
	objectTagMap.insert({ "jelly", 500 });
	objectTagMap.insert({ "demon", 600 });
	objectTagMap.insert({ "key", 700 });
	objectTagMap.insert({ "box", 800 });

	createAnimation();
	createMap();

    return true;
}

void HelloWorld::createMap() {
	stageLayer = LayerColor::create(stageColor.at(stageNum - 1));
	this->addChild(stageLayer, 0, 1);

	if (stageNum == 1) {
		auto howTo = LabelTTF::create(
			"칼은 늑대를 처치할 수 있으며, 메이스는 젤리를 처치할 수 있고, 성검으로 악마를 처치할 수 있습니다."\
			" 열쇠로 상자를 열면 클리어입니다. 단, 한번에 하나의 아이템만을 가질 수 있습니다."
			, "Arial", 16);
	
		howTo->setPosition(Vec2(1000, 10));
		howTo->setColor(Color3B::WHITE);
		howTo->runAction(RepeatForever::create(
			Sequence::create(
				MoveTo::create(12.0f, Vec2(-howTo->getContentSize().width, 10)),
				Place::create(Vec2(1000, 10)),
				NULL)));
		stageLayer->addChild(howTo);
	}

	char buf[20] = { 0 };

	sprintf(buf, "Stage %d", stageNum);
	stage->setString(buf);

	sprintf(buf, "TileMaps/Stage%d.tmx", stageNum);
	tmap = TMXTiledMap::create(buf);

	metainfo = tmap->getLayer("MetaInfo");
	indexinfo = tmap->getLayer("IndexInfo");
	metainfo->setVisible(false);
	indexinfo->setVisible(false);

	stageLayer->addChild(tmap, 0);

	TMXObjectGroup* objects = tmap->getObjectGroup("Objects");

	heroPosition = Vec2(objects->getObject("HeroPoint")["x"].asInt(), objects->getObject("HeroPoint")["y"].asInt());
	keyPosition = Vec2(objects->getObject("KeyPoint")["x"].asInt(), objects->getObject("KeyPoint")["y"].asInt());
	boxPosition = Vec2(objects->getObject("BoxPoint")["x"].asInt(), objects->getObject("BoxPoint")["y"].asInt());

	int n = 1;

	while (1) {
		sprintf(buf, "WolfPoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		wolfPosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	n = 1;

	while (1) {
		sprintf(buf, "JellyPoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		jellyPosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	n = 1;

	while (1) {
		sprintf(buf, "DemonPoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		demonPosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	n = 1;

	while (1) {
		sprintf(buf, "SwordPoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		swordPosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	n = 1;

	while (1) {
		sprintf(buf, "MacePoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		macePosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	n = 1;

	while (1) {
		sprintf(buf, "HolySwordPoint%d", n);

		if (objects->getObject(buf)["x"].asInt() == 0) {
			break;
		}
		holySwordPosition.push_back(Vec2(objects->getObject(buf)["x"].asInt(), objects->getObject(buf)["y"].asInt()));
		n++;
	}

	this->createObject();
}

void HelloWorld::createObject() {
	hero = Sprite::createWithSpriteFrameName("Hero02.png");
	hero->setPosition(heroPosition);
	stageLayer->addChild(hero);

	for (int i = 0; i < wolfPosition.size(); i++) {
		wolf.pushBack(Sprite::createWithSpriteFrameName("Wolf2.png"));
		wolf.at(i)->setPosition(wolfPosition.at(i));
		wolf.at(i)->setScale(0.8f);
		stageLayer->addChild(wolf.at(i), 0, 400 + i);

		wolf.at(i)->runAction(RepeatForever::create(Animate::create(wolfAnimation)));
	}

	for (int i = 0; i < jellyPosition.size(); i++) {
		jelly.pushBack(Sprite::createWithSpriteFrameName("Jelly2.png"));
		jelly.at(i)->setPosition(jellyPosition.at(i));
		jelly.at(i)->setScale(0.8f);
		stageLayer->addChild(jelly.at(i), 0, 500 + i);

		jelly.at(i)->runAction(RepeatForever::create(Animate::create(jellyAnimation)));
	}

	for (int i = 0; i < demonPosition.size(); i++) {
		demon.pushBack(Sprite::createWithSpriteFrameName("Demon2.png"));
		demon.at(i)->setPosition(demonPosition.at(i));
		demon.at(i)->setScale(0.8f);
		stageLayer->addChild(demon.at(i), 0, 600 + i);

		demon.at(i)->runAction(RepeatForever::create(Animate::create(demonAnimation)));
	}

	// 아이템
	box = Sprite::createWithSpriteFrameName("I_Chest01.png");
	key = Sprite::createWithSpriteFrameName("I_Key02.png");

	box->setScale(0.8f);
	key->setScale(0.8f);

	box->setPosition(boxPosition);
	key->setPosition(keyPosition);

	stageLayer->addChild(box, 0, 800);
	stageLayer->addChild(key, 0, 700);

	for (int i = 0; i < swordPosition.size(); i++) {
		sword.pushBack(Sprite::createWithSpriteFrameName("W_Sword001.png"));
		sword.at(i)->setPosition(swordPosition.at(i));
		stageLayer->addChild(sword.at(i), 0, 100 + i);
	}
	
	for (int i = 0; i < macePosition.size(); i++) {
		mace.pushBack(Sprite::createWithSpriteFrameName("W_Mace008.png"));
		mace.at(i)->setPosition(macePosition.at(i));
		stageLayer->addChild(mace.at(i), 0, 200 + i);
	}

	for (int i = 0; i < holySwordPosition.size(); i++) {
		holySword.pushBack(Sprite::createWithSpriteFrameName("W_Sword015.png"));
		holySword.at(i)->setPosition(holySwordPosition.at(i));
		stageLayer->addChild(holySword.at(i), 0, 300 + i);
	}
}

void HelloWorld::createAnimation() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Holygrail.plist");

	// 플레이어 상하좌우 애니메이션 만들기
	heroUpAnimation = Animation::create();
	heroUpAnimation->setDelayPerUnit(0.1f);
	heroUpAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero10.png"));
	heroUpAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero12.png"));
	heroUpAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero11.png"));
	heroUpAnimation->retain();

	heroDownAnimation = Animation::create();
	heroDownAnimation->setDelayPerUnit(0.1f);
	heroDownAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero01.png"));
	heroDownAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero03.png"));
	heroDownAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero02.png"));
	heroDownAnimation->retain();

	heroLeftAnimation = Animation::create();
	heroLeftAnimation->setDelayPerUnit(0.1f);
	heroLeftAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero04.png"));
	heroLeftAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero06.png"));
	heroLeftAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero05.png"));
	heroLeftAnimation->retain();

	heroRightAnimation = Animation::create();
	heroRightAnimation->setDelayPerUnit(0.1f);
	heroRightAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero07.png"));
	heroRightAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero09.png"));
	heroRightAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero08.png"));
	heroRightAnimation->retain();

	// 늑대 애니메이션
	wolfAnimation = Animation::create();
	wolfAnimation->setDelayPerUnit(0.2f);
	wolfAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Wolf1.png"));
	wolfAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Wolf2.png"));
	wolfAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Wolf3.png"));
	wolfAnimation->retain();

	// 젤리 애니메이션
	jellyAnimation = Animation::create();
	jellyAnimation->setDelayPerUnit(0.2f);
	jellyAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Jelly1.png"));
	jellyAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Jelly2.png"));
	jellyAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Jelly3.png"));
	jellyAnimation->retain();

	// 악마 애니메이션
	demonAnimation = Animation::create();
	demonAnimation->setDelayPerUnit(0.2f);
	demonAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Demon1.png"));
	demonAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Demon2.png"));
	demonAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("Demon3.png"));
	demonAnimation->retain();
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	Vec2 playerPos = hero->getPosition();

	Vec2 diff = touchPoint - playerPos;

	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			hero->runAction(Animate::create(heroRightAnimation));
			playerPos.x = playerPos.x + tmap->getTileSize().width;
		}
		else {
			hero->runAction(Animate::create(heroLeftAnimation));
			playerPos.x = playerPos.x - tmap->getTileSize().width;
		}
	}
	else {
		if (diff.y > 0) {
			hero->runAction(Animate::create(heroUpAnimation));
			playerPos.y = playerPos.y + tmap->getTileSize().height;
		}
		else {
			hero->runAction(Animate::create(heroDownAnimation));
			playerPos.y = playerPos.y - tmap->getTileSize().height;
		}
	}

	if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width)
		&& playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height)
		&& playerPos.y >= 0
		&& playerPos.x >= 0) {
		this->setPlayerPosition(playerPos);
	}
}

Vec2 HelloWorld::tileCoordForPosition(Vec2 position) {
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y)
		/ tmap->getTileSize().height;

	return Vec2(x, y);
}

void HelloWorld::setPlayerPosition(Vec2 position) {
	Vec2 tileCoord = this->tileCoordForPosition(position);
	int metaTileGid = metainfo->getTileGIDAt(tileCoord);
	int indexTildGid = indexinfo->getTileGIDAt(tileCoord);

	if (metaTileGid) {
		Value& metaProperties = tmap->getPropertiesForGID(metaTileGid);
		Value& indexProperties = tmap->getPropertiesForGID(indexTildGid);

		if (!metaProperties.isNull()) {
			std::string wall = metaProperties.asValueMap()["Wall"].asString();

			if (wall == "YES") {
				log("Wall");
				return;
			}

			int index = 0;

			if (!indexProperties.isNull()) {
				index = indexProperties.asValueMap()["Index"].asInt();
			}

			std::string item = metaProperties.asValueMap()["Item"].asString();
			
			if (item != "") { // 아이템 획득
				haveItem = item;
				metainfo->removeTileAt(tileCoord);
				stageLayer->removeChildByTag(objectTagMap[haveItem] + index);

				itemBox->setSpriteFrame(
					SpriteFrameCache::getInstance()->getSpriteFrameByName(itemFrameMap[item]));
			}

			std::string obj = metaProperties.asValueMap()["Obj"].asString();

			if ((obj != "") && (haveItem != "")) { // 해당 아이템을 가지고 있을 경우
				if (objectMap[haveItem] == obj) {
					metainfo->removeTileAt(tileCoord);

					itemBox->setSpriteFrame(SpriteFrame::createWithTexture(
						texture,
						Rect(texture->getContentSize().width - 46, texture->getContentSize().height - 45, 46, 45)));

					if (obj == "box") {
						box->setSpriteFrame(
							SpriteFrameCache::getInstance()->getSpriteFrameByName("I_Chest02.png"));
						
						stageNum++;
						initMap(this);

						return;
					}
					stageLayer->removeChildByTag(objectTagMap[obj] + index);
				}
				else {
					return;
				}
			}
		}
	}
	hero->setPosition(position);
}

void HelloWorld::initMap(Ref* pSender) {
	if (stageNum == 4) {
		return;
	}

	this->removeChildByTag(1);

	hero->release();
	box->release();

	wolf.clear();
	jelly.clear();
	demon.clear();
	sword.clear();
	mace.clear();
	holySword.clear();

	wolfPosition.clear();
	jellyPosition.clear();
	demonPosition.clear();
	swordPosition.clear();
	macePosition.clear();
	holySwordPosition.clear();

	createMap();
}

