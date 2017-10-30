#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

struct GemInfo {
	int x;
	int y;
	int gemType;
};

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

	// 헤더 (타임바 부분) 이미지 추가
	auto pHeader = Sprite::create("gamescene/resources-auto/header.png");
	pHeader->setPosition(Vec2(320, 900));
	this->addChild(pHeader, 5);

	timer = ProgressTimer::create(Sprite::create("gamescene/resources-auto/timer.png"));
	timer->setPercentage(100);
	timer->setType(ProgressTimer::Type::BAR);
	timer->setPosition(Vec2(pHeader->getContentSize().width / 2, pHeader->getContentSize().height / 3 - 8));
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 0.5));
	pHeader->addChild(timer);

	// 이펙트 추가
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("crystals/resources-auto/sparkle.png", Rect(0, 0, 32, 32)),
		"sparkle.png");

	blend.src = GL_SRC_ALPHA;
	blend.dst = GL_ONE;

	setupBoard();


	return true;
}

void GameScene::setupBoard() {
	timeLeft = 99.9f;

	createGem();

	schedule(schedule_selector(GameScene::update));
	schedule(schedule_selector(GameScene::clock), 1.f);
}

void GameScene::update(float t) {
	if (!isGameOver) {
		testCreateGem();

		updateSparkle();

		if ((lastMoveTime > 5) && !isDisplayingHint) {
			displayHint();
		}

		if (timeLeft < 0.f) {
			timeLeft = 0.f;
		}

		if (timeLeft > 99.9f) {
			timeLeft = 99.9f;
		}

		timer->setPercentage(timeLeft);

		if (timeLeft == 0) {
			//createGameOver();
			isGameOver = true;
			lastScore = score;
		}
	}
}

void GameScene::clock(float t) {
	lastMoveTime++;
	timeLeft--;
}

bool GameScene::createGem() {
	isCreated = false;

	char str[50] = { 0 };

	for (int i = 0; i < HEIGHT_NUM; i++) {
		for (int j = 0; j < WIDTH_NUM; j++) {
			int randNum = rand() % 5;
			sprintf(str, "crystals/resources-auto/%d.png", randNum);

			board[i * WIDTH_NUM + j] = Sprite::create(str);
			board[i * WIDTH_NUM + j]->retain();
			board[i * WIDTH_NUM + j]->setTag(randNum);
			board[i * WIDTH_NUM + j]->setScale(0.8f);
			board[i * WIDTH_NUM + j]->setAnchorPoint(Vec2::ZERO);
			board[i * WIDTH_NUM + j]->setPosition(Vec2(80 * j + 10, this->getContentSize().height + 120 + 80 * i));
			log("board[%d] : %f. %f",
				i * WIDTH_NUM + j,
				board[i * WIDTH_NUM + j]->getPosition().x,
				board[i * WIDTH_NUM + j]->getPosition().y);
			this->addChild(board[i * WIDTH_NUM + j]);

			board[i * WIDTH_NUM + j]->runAction(MoveTo::create(1.3f,
				Vec2(board[i * WIDTH_NUM + j]->getPosition().x, 80 * i)));
		}
	}
	isCreated = true;

	return isCreated;
}

void GameScene::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	lastMoveTime = 0;

	// 힌트였던 보석의 경우
	if (this->getChildByTag(200) != NULL) {
		while (this->getChildByTag(200) != NULL) {
			this->removeChildByTag(200);
		}

		connectedGems.clear();
	}

	for (int i = 0; i < HEIGHT_NUM; i++) {
		for (int j = 0; j < WIDTH_NUM; j++) {
			if (board[i * WIDTH_NUM + j]->getBoundingBox().containsPoint(touch->getLocation())) {
				if (board[i * WIDTH_NUM + j]->getTag() == 5) {
					activatePowerUp(i, j, 0, 0);
				}
				else {
					removeConnectedGems(i, j, board[i * WIDTH_NUM + j]->getTag());
				}
			}
		}
	}

	return true;
}

void GameScene::removeConnectedGems(int x, int y, int gemType) {
	isRemoved = false;

	if ((x < 0) || (x >= HEIGHT_NUM)) return;
	if ((y < 0) || (y >= WIDTH_NUM)) return;

	findConnectedGems(x, y, gemType);

	if (connectedGems.size() >= 3) {
		if (connectedGems.size() >= 6) {
			idxPup = connectedGems.at(rand() % connectedGems.size());
			
			idxPup->setOpacity(0);

			for (auto gem : connectedGems) {
				if (gem != idxPup) {
					auto pLight = Sprite::create("crystals/resources-auto/bomb-light.png");
					pLight->setPosition(gem->getPosition());
					pLight->setAnchorPoint(Vec2::ZERO);

					pLight->setBlendFunc(blend);
					this->addChild(pLight);
					pLight->runAction(Sequence::create(
						MoveTo::create(0.2f, idxPup->getPosition()),
						CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, pLight)),
						NULL));
				}
			}
		}


		for (auto gem : connectedGems) {
			ParticleSystem* emitter = ParticleSystemQuad::create("particles/taken-gem.plist");
			emitter->setPosition(gem->getPosition());
			emitter->setAutoRemoveOnFinish(true);

			this->addChild(emitter);

			if (gem == idxPup) {
				log("널 타고 있니?");
				gem->setOpacity(255);
				gem->setTexture("crystals/resources-auto/bomb.png");
				gem->setTag(5);

				auto bombGlow = Sprite::create("crystals/resources-auto/bomb-hi.png");
				bombGlow->setTag(6);
				bombGlow->setAnchorPoint(Vec2(0, 0));
				gem->addChild(bombGlow);
				bombGlow->runAction(RepeatForever::create(
					Sequence::create(FadeOut::create(0.4f), FadeIn::create(0.4f), NULL)));
			}
			else {
				this->removeChild(gem, false);
				gem->setTag(-1);
			}
		}

		timeLeft++;
	}

	isDisplayingHint = false;

	connectedGems.clear();
	log("삭제됨");
	isRemoved = true;
}

void GameScene::findConnectedGems(int x, int y, int gemType) {
	isFinding = true;

	std::vector<GemInfo> findIndexs;

	findIndexs.push_back({ x, y, gemType });

	while (1) {
		bool isVisited = false;

		if (findIndexs.size() == 0) {
			log("findIdx 사이즈 0");
			break;
		}

		GemInfo idx = findIndexs.back();
		log("idx.x : %d, idx.y : %d", idx.x, idx.y);
		findIndexs.pop_back();

		if (isRemoved) continue;
	
		if ((idx.x < 0) || (idx.x >= HEIGHT_NUM)) continue;

		if ((idx.y < 0) || (idx.y >= WIDTH_NUM)) continue;

		if (board[idx.x * WIDTH_NUM + idx.y]->getTag() != idx.gemType) {
			log("board[%d]->getTag() != %d 해당", idx.x * WIDTH_NUM + idx.y, idx.gemType);
			continue;
		}

		for (auto gem : connectedGems) {
			if (board[idx.x * WIDTH_NUM + idx.y] == gem) {
				log("board[%d] 이미 방문", idx.x * WIDTH_NUM + idx.y);
				isVisited = true;
				break;
			}
		}

		if (isVisited) continue;

		connectedGems.pushBack(board[idx.x * WIDTH_NUM + idx.y]);
		log("인덱스 : %d (%d, %d), 태그 - %d", idx.x * WIDTH_NUM + idx.y, idx.x, idx.y, idx.gemType);

		findIndexs.push_back({ idx.x + 1, idx.y, idx.gemType });
		findIndexs.push_back({ idx.x - 1, idx.y, idx.gemType });
		findIndexs.push_back({ idx.x, idx.y + 1, idx.gemType });
		findIndexs.push_back({ idx.x, idx.y - 1, idx.gemType });
	}

	isFinding = false;

}

void GameScene::testCreateGem() {
	if (!isChanging) {
		for (int i = 0; i < HEIGHT_NUM; i++) {
			for (int j = 0; j < WIDTH_NUM; j++) {
				if (isRemoved) {
					if (board[i * WIDTH_NUM + j]->getTag() == -1) {
						isChanging = true;
						log("현재 board[%d]", i * WIDTH_NUM + j);

						int idx = i;
						int col = i + 1;
						Vec2 nullGemPos = Vec2::ZERO;

						while (idx < HEIGHT_NUM - 1) {

							// 위에서 내릴 보석 찾기
							while ((board[col * WIDTH_NUM + j]->getTag() == -1)) {
								col++;

								if (col > 8) {
									break;
								}
							}
							log("idx : %d col : %d 꼭대기가 아님", idx, col);

							if ((col > 9)
								|| ((col == 9) && ((board[col * WIDTH_NUM + j]->getTag() == -1)))) {
								break;
							}

							log(" 과정 board[%d] <-> board[%d]", idx * WIDTH_NUM + j, col * WIDTH_NUM + j);
							this->removeChild(board[col * WIDTH_NUM + j], false);
							nullGemPos = board[idx * WIDTH_NUM + j]->getPosition();

							log("이동할 위치 : %f, %f", nullGemPos.x, nullGemPos.y);
							board[idx * WIDTH_NUM + j]->setTexture(board[col * WIDTH_NUM + j]->getTexture());
							board[idx * WIDTH_NUM + j]->setPosition(board[col * WIDTH_NUM + j]->getPosition());
							board[idx * WIDTH_NUM + j]->setTag(board[col * WIDTH_NUM + j]->getTag());

							if (board[col * WIDTH_NUM + j]->getChildByTag(6) != NULL) {
								log("자식 교환");
								auto bombChild = board[col * WIDTH_NUM + j]->getChildByTag(6);
								board[col * WIDTH_NUM + j]->removeChildByTag(6, false);
								board[idx * WIDTH_NUM + j]->addChild(bombChild);

							}

							board[col * WIDTH_NUM + j]->setTag(-1);

							this->addChild(board[idx * WIDTH_NUM + j]);
							board[idx * WIDTH_NUM + j]->runAction(MoveTo::create(0.3f, nullGemPos));
							log("board[%d] 얘랑 바껴", col * WIDTH_NUM + j);
							log("태그 : %d, %d", board[idx * WIDTH_NUM + j]->getTag(), board[col * WIDTH_NUM + j]->getTag());

							if (idx == 8) {
								idx++;
								break;
							}
							idx++;
						}

						log("꼭대기");

						log("idx : %d", idx);
						char str[50] = { 0 };
						int randNum = rand() % 5;

						nullGemPos = board[idx * WIDTH_NUM + j]->getPosition();
						sprintf(str, "crystals/resources-auto/%d.png", randNum);
						log("이동할 위치 : %f, %f", nullGemPos.x, nullGemPos.y);

						board[idx * WIDTH_NUM + j]->setTexture(str);
						board[idx * WIDTH_NUM + j]->setTag(randNum);
						board[idx * WIDTH_NUM + j]->setPosition(Vec2(80 * j + 10, this->getContentSize().height + 120 + 80.f * idx));

						this->addChild(board[idx * WIDTH_NUM + j]);

						board[idx * WIDTH_NUM + j]->runAction(Sequence::create(
							MoveTo::create(0.3f, nullGemPos),
							CallFunc::create(CC_CALLBACK_0(GameScene::endAction, this, idx, j)),
							NULL));
						log("액션 : %f, %f",
							nullGemPos.x,
							nullGemPos.y);
					}
				}
			}
		}
	}
}

void GameScene::endAction(int col, int j) {
	isChanging = false;
	log("board[%d] : %f. %f tag - %d",
		col * WIDTH_NUM + j,
		board[col * WIDTH_NUM + j]->getPosition().x,
		board[col * WIDTH_NUM + j]->getPosition().y,
		board[col * WIDTH_NUM + j]->getTag());
}

void GameScene::displayHint() {
	isDisplayingHint = true;
	isRemoved = false;

	int x = rand() % HEIGHT_NUM;
	int y = rand() % WIDTH_NUM;

	log("board[%d] / x : %d, y : %d tag : %d", x * WIDTH_NUM + y, x, y, board[x * WIDTH_NUM + y]->getTag());
	findConnectedGems(x, y, board[x * WIDTH_NUM + y]->getTag());

	if (!isFinding) {
		log("1");
		if (connectedGems.size() < 3) {
			log("2");
			connectedGems.clear();
			isDisplayingHint = false;
		}
		else {
			log("3");
			for (auto gem : connectedGems) {
				auto pSprite = Sprite::create("crystals/resources-auto/hint.png");
				pSprite->setOpacity(0);
				pSprite->setAnchorPoint(Vec2(0.1, 0.1));
				pSprite->setPosition(gem->getPosition());
				pSprite->setTag(200);

				this->addChild(pSprite, 1);
				pSprite->runAction(RepeatForever::create(Sequence::create(
					FadeIn::create(0.5f), FadeOut::create(0.5f), NULL)));
			}
		}
	}
}

void GameScene::activatePowerUp(int x, int y, int hStart, int vStart) {
	isRemoved = false;
	if (board[x * WIDTH_NUM + y]->getChildByTag(6) != NULL) {
		board[x * WIDTH_NUM + y]->removeChildByTag(6, true);
	}

	auto pHorizontal0 = Sprite::create("crystals/resources-auto/bomb-explo.png");
	pHorizontal0->setBlendFunc(blend);
	pHorizontal0->setPosition(Vec2(0, board[x * WIDTH_NUM + y]->getPosition().y));
	pHorizontal0->setAnchorPoint(Vec2::ZERO);
	pHorizontal0->setScaleX(5);


	auto pVertical0 = Sprite::create("crystals/resources-auto/bomb-explo.png");
	pVertical0->setBlendFunc(blend);
	pVertical0->setPosition(Vec2(board[x * WIDTH_NUM + y]->getPosition().x, 0));
	pVertical0->setAnchorPoint(Vec2::ZERO);
	pVertical0->setScaleY(5);
	
	auto pHorizontal1 = Sprite::create("crystals/resources-auto/bomb-explo-inner.png");
	pHorizontal1->setBlendFunc(blend);
	pHorizontal1->setPosition(Vec2(0, board[x * WIDTH_NUM + y]->getPosition().y));
	pHorizontal1->setAnchorPoint(Vec2::ZERO);
	pHorizontal1->setScaleX(0.5);

	auto pVertical1 = Sprite::create("crystals/resources-auto/bomb-explo-inner.png");
	pVertical1->setRotation(90);
	pVertical1->setBlendFunc(blend);
	pVertical1->setPosition(Vec2(board[x * WIDTH_NUM + y]->getPosition().x, 0));
	pVertical1->setAnchorPoint(Vec2::ZERO);
	pVertical1->setScaleY(0.5);

	this->addChild(pHorizontal0);
	this->addChild(pVertical0);
	this->addChild(pHorizontal1);
	this->addChild(pVertical1);

	pHorizontal0->runAction(Sequence::create(
		ScaleTo::create(0.5f, 30, 1),
		FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, pHorizontal0)),
		NULL));

	pVertical0->runAction(Sequence::create(
		ScaleTo::create(0.5f, 1, 30),
		FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, pVertical0)),
		NULL));

	pHorizontal1->runAction(Sequence::create(
		ScaleTo::create(0.5f, 8, 1),
		FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, pHorizontal1)),
		NULL));

	pVertical1->runAction(Sequence::create(
		ScaleTo::create(0.5f, 1, 8),
		FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, pVertical1)),
		NULL));

	for (int i = vStart; i < HEIGHT_NUM; i++) {
		if ((board[i * WIDTH_NUM + y]->getTag() == 5) && (i != x)) {
			activatePowerUp(i, y, 0, i);
		}
		else {
			this->removeChild(board[i * WIDTH_NUM + y], false);
			board[i * WIDTH_NUM + y]->setTag(-1);
			log("board[%d] -1로 만듦", i * WIDTH_NUM + y);
		}
	}

	for (int i = hStart; i < WIDTH_NUM; i++) {
		if (y != i) {
			if (board[x * WIDTH_NUM + i]->getTag() == 5) {
				activatePowerUp(x, i, i, 0);
			}
			else {
				this->removeChild(board[x * WIDTH_NUM + i], false);
				board[x * WIDTH_NUM + i]->setTag(-1);
				log("board[%d] -1로 만듦", x * WIDTH_NUM + i);
			}
		}
	}

	idxPup = nullptr;
	isRemoved = true;
}

void GameScene::doRemoveFromParent(Ref* pSender) {
	this->removeChild((Sprite*)pSender);
}

void GameScene::updateSparkle() {
	if (rand() % 100 < 10) {
		Sprite* sparkGem = board[rand() % 80];

		if ((sparkGem->getTag() < 0) || (sparkGem->getTag() >= 5)) return;
		if (!sparkGem) return;
		if (sparkGem->getChildrenCount() > 0) return;

		auto sparkle = Sprite::createWithSpriteFrameName("sparkle.png");
		sparkle->setOpacity(0);

		sparkle->runAction(RepeatForever::create(
			Spawn::create(
				RotateBy::create(3, 360),
				Sequence::create(
					FadeIn::create(0.5f),
					FadeOut::create(2),
					CallFunc::create(CC_CALLBACK_0(GameScene::removeFromParent, sparkle)),
					NULL),
				NULL)));

		sparkGem->addChild(sparkle);
	}
}