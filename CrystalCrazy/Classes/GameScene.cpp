#include "GameScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

// ������ ����
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

	/////////////////////////////////
	winSize = Director::getInstance()->getWinSize();
	srand((int)time(NULL));

	// ��� �̹��� �߰�
	auto pBackgroud = Sprite::create("gamescene/resources-auto/background.png");
	pBackgroud->setPosition(Vec2(320, 480));
	this->addChild(pBackgroud);

	// ��� �̹��� �߰�
	pHeader = Sprite::create("gamescene/resources-auto/header.png");
	pHeader->setPosition(Vec2(winSize.width / 2, winSize.height + 200));
	pHeader->setScale(0.8f);
	this->addChild(pHeader, 5);
	pHeader->runAction(Sequence::create(
		MoveTo::create(3.5f, Vec2(winSize.width / 2, winSize.height - 50)),
		CallFunc::create(CC_CALLBACK_0(GameScene::gameStart, this)),
		NULL));

	// Ÿ�ӹ� �߰�
	timer = ProgressTimer::create(Sprite::create("gamescene/resources-auto/timer.png"));
	timer->setPercentage(100);
	timer->setType(ProgressTimer::Type::BAR);
	timer->setPosition(Vec2(pHeader->getContentSize().width / 2, pHeader->getContentSize().height / 3 - 8));
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 0.5));
	pHeader->addChild(timer);

	// ���� �߰�
	pScore = LabelBMFont::create("0", "fonts/scorefont.fnt");
	pScore->setAnchorPoint(Vec2(1, 1));
	pScore->setPosition(Vec2(pHeader->getContentSize().width - 20, pHeader->getContentSize().height - 30));
	pHeader->addChild(pScore);

	// �Ͻ� ���� ��ư �߰�
	auto pPauseBtn = MenuItemImage::create(
		"gamescene/resources-auto/btn-pause.png",
		"gamescene/resources-auto/btn-pause-down.png",
		CC_CALLBACK_1(GameScene::gameOver, this));
	pPauseBtn->setScale(0.7f);

	auto pMenu = Menu::create(pPauseBtn, NULL);
	pMenu->setPosition(Vec2(25, pHeader->getContentSize().height - 45));
	pMenu->setAnchorPoint(Vec2(1, 0));
	pHeader->addChild(pMenu);

	// ����Ʈ �߰�
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("crystals/resources-auto/sparkle.png", Rect(0, 0, 32, 32)),
		"sparkle.png");

	blend.src = GL_SRC_ALPHA;
	blend.dst = GL_ONE;

	setupBoard();

	return true;
}

/*
	���� ���� �� �� �⺻���� ����
*/
void GameScene::setupBoard() {
	timeLeft = 99.9f;

	createGem();
	//createShimmer();

	schedule(schedule_selector(GameScene::update));
	schedule(schedule_selector(GameScene::clock), 1.f);
}

void GameScene::gameStart() {
	auto goSprite = Sprite::create("gamescene/resources-auto/go.png");
	goSprite->setScale(0);
	goSprite->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(goSprite);

	goSprite->runAction(
		Sequence::create(
			ScaleTo::create(1, 1.0f),
			ScaleTo::create(1, 0.f),
			CallFunc::create(CC_CALLBACK_0(GameScene::endAction, this)),
			nullptr
		));
}

void GameScene::gameOver(Ref* pSender) {
	isGameOver = true;

	if (overCnt == 0) {
		pHeader->runAction(MoveTo::create(2.0f, Vec2(winSize.width / 2, winSize.height + 200)));
		
		while (this->getChildByTag(20) != nullptr) {
			this->removeChildByTag(20);
		}
	}

	for (int i = 0; i < HEIGHT_NUM; i++) {
		for (int j = 0; j < WIDTH_NUM; j++) {
			float randx = rand() % 50 * 0.2;
			float randy = rand() % 50 * 0.2;

			if (i % 2 == 0) randx *= -1;
			if (overCnt > 25) randy *= -1;
			
			board[i * WIDTH_NUM + j]->setPosition(
				Vec2(board[i * WIDTH_NUM + j]->getPosition().x + randx,
					board[i * WIDTH_NUM + j]->getPosition().y + randy));
		}
	}

	for (int i = 0; i < HEIGHT_NUM; i++) {
		for (int j = 0; j < WIDTH_NUM; j++) {
			if ((board[i * WIDTH_NUM + j]->getPosition().x < 0)
				|| (board[i * WIDTH_NUM + j]->getPosition().x > WIDTH_NUM)
				|| (board[i * WIDTH_NUM + j]->getPosition().y < 0)
				|| (board[i * WIDTH_NUM + j]->getPosition().y > HEIGHT_NUM)) {
				board[i * WIDTH_NUM + j]->setTag(-2);
			}
		}
	}

	if (overCnt == 100) {
		Director::getInstance()->replaceScene(MainScene::createScene());
	}

	overCnt++;
}

void GameScene::update(float t) {
	if (isStartGame && !isGameOver) {
		updateGem();
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
			gameOver(this);
			lastScore = score_;
		}
	}

	if (isGameOver) {
		gameOver(this);
	}
}

void GameScene::clock(float t) {
	if (isStartGame) {
		lastMoveTime++;
		timeLeft--;
	}
}

/*
	���� ���� �� ���õǴ� ������
*/
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
			board[i * WIDTH_NUM + j]->setPosition(Vec2(65 * j, winSize.height + 120 + 65 * i));

			this->addChild(board[i * WIDTH_NUM + j]);
			this->setZOrder(3);
			// ������ �������� �ӵ��� �޸��ؼ� ���� ���� ���̰� ��
			float speed = (i > 0) ? i - (0.65 * i) : i;
			board[i * WIDTH_NUM + j]->runAction(MoveTo::create(0.5f + speed,
				Vec2(board[i * WIDTH_NUM + j]->getPosition().x, 65 * i)));
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

	// ��Ʈ���� ������ Ŭ���� ���
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

/*
	����� ���� ���� ����
	int x : ���� �ε���
	int y : ���� �ε���
	int gemType : ������ ����(0 ~ 4)
*/
void GameScene::removeConnectedGems(int x, int y, int gemType) {
	isRemoved = false;

	if ((x < 0) || (x >= HEIGHT_NUM)) return;
	if ((y < 0) || (y >= WIDTH_NUM)) return;

	findConnectedGems(x, y, gemType);

	if (connectedGems.size() >= 3) {
		addScore(100 * connectedGems.size());
		if (connectedGems.size() >= 6) {
			addScore(2000);

			// ����� ���� �߿� �ϳ��� �������� �̾Ƽ� ��ź �ڸ��� ����
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

	connectedGems.clear();

	isDisplayingHint = false;
	isRemoved = true;
}

/*
	����� ���� ������ ���� ã��
	int x : ���� �ε���
	int y : ���� �ε���
	int gemType : ������ ����(0 ~ 4)
*/
void GameScene::findConnectedGems(int x, int y, int gemType) {
	isFinding = true;

	std::vector<GemInfo> findIndexs;

	findIndexs.push_back({ x, y, gemType });

	while (1) {
		bool isVisited = false;

		// ���̻� ����� ���� ����
		if (findIndexs.size() == 0) {
			break;
		}

		GemInfo idx = findIndexs.back();
		findIndexs.pop_back();

		if (isRemoved) continue;

		if ((idx.x < 0) || (idx.x >= HEIGHT_NUM)) continue;
		if ((idx.y < 0) || (idx.y >= WIDTH_NUM)) continue;

		// ���� ������ ������ ������ ������ ���� ����
		if (board[idx.x * WIDTH_NUM + idx.y]->getTag() != idx.gemType) continue;

		// �湮 ���� üũ
		for (auto gem : connectedGems) {
			if (board[idx.x * WIDTH_NUM + idx.y] == gem) {
				isVisited = true;
				break;
			}
		}

		if (isVisited) continue;

		connectedGems.pushBack(board[idx.x * WIDTH_NUM + idx.y]);

		findIndexs.push_back({ idx.x + 1, idx.y, idx.gemType });
		findIndexs.push_back({ idx.x - 1, idx.y, idx.gemType });
		findIndexs.push_back({ idx.x, idx.y + 1, idx.gemType });
		findIndexs.push_back({ idx.x, idx.y - 1, idx.gemType });
	}

	isFinding = false;
}

/*
	������ ���� �� ���� ���� ������
*/
void GameScene::updateGem() {
	if (!isChanging) {
		for (int i = 0; i < HEIGHT_NUM; i++) {
			for (int j = 0; j < WIDTH_NUM; j++) {
				if (isRemoved) {
					// ���ŵ� �ڸ��� ���
					if (board[i * WIDTH_NUM + j]->getTag() == -1) {
						isChanging = true;

						int idx = i;
						int col = i + 1;
						Vec2 nullGemPos = Vec2::ZERO;

						while (idx < HEIGHT_NUM - 1) {
							// �������� ���� ���� ã��(���ŵ��� ���� ����)
							while ((board[col * WIDTH_NUM + j]->getTag() == -1)) {
								col++;

								if (col > 8) {
									break;
								}
							}

							if ((col > 9)
								|| ((col == 9) && ((board[col * WIDTH_NUM + j]->getTag() == -1)))) {
								break;
							}

							this->removeChild(board[col * WIDTH_NUM + j], false);
							nullGemPos = board[idx * WIDTH_NUM + j]->getPosition();

							// ���ŵ� ��ġ ������ ������ �������� ���� ������ ���� ��ȯ
							board[idx * WIDTH_NUM + j]->setTexture(board[col * WIDTH_NUM + j]->getTexture());
							board[idx * WIDTH_NUM + j]->setPosition(board[col * WIDTH_NUM + j]->getPosition());
							board[idx * WIDTH_NUM + j]->setTag(board[col * WIDTH_NUM + j]->getTag());

							// ��ź(�ڽ�) ��ȯ
							if (board[col * WIDTH_NUM + j]->getChildByTag(6) != NULL) {
								auto bombChild = board[col * WIDTH_NUM + j]->getChildByTag(6);
								board[col * WIDTH_NUM + j]->removeChildByTag(6, false);
								board[idx * WIDTH_NUM + j]->addChild(bombChild);

							}

							board[col * WIDTH_NUM + j]->setTag(-1);

							this->addChild(board[idx * WIDTH_NUM + j]);
							board[idx * WIDTH_NUM + j]->runAction(MoveTo::create(0.3f, nullGemPos));

							if (idx == 8) {
								idx++;
								break;
							}
							idx++;
						}

						// ������� ��� ������ ������ �����Ƿ� ��ü������ ������ ������ �����ؼ� ��������
						char str[50] = { 0 };
						int randNum = rand() % 5;

						nullGemPos = board[idx * WIDTH_NUM + j]->getPosition();
						sprintf(str, "crystals/resources-auto/%d.png", randNum);

						board[idx * WIDTH_NUM + j]->setTexture(str);
						board[idx * WIDTH_NUM + j]->setTag(randNum);
						board[idx * WIDTH_NUM + j]->setPosition(Vec2(65 * j, winSize.height + 120 + 65 * idx));

						this->addChild(board[idx * WIDTH_NUM + j]);

						board[idx * WIDTH_NUM + j]->runAction(Sequence::create(
							MoveTo::create(0.3f, nullGemPos),
							CallFunc::create(CC_CALLBACK_0(GameScene::endAction, this)),
							NULL));
					}
				}
			}
		}
	}
}

/*
	�ݹ� �׼ǿ��� �׼� ������ ����Ǵ� �Լ�
*/
void GameScene::endAction() {
	isStartGame = true;
	isChanging = false;
}

/*
	���� ��Ʈ ǥ��
*/
void GameScene::displayHint() {
	isDisplayingHint = true;
	isRemoved = false;

	int x = rand() % HEIGHT_NUM;
	int y = rand() % WIDTH_NUM;

	findConnectedGems(x, y, board[x * WIDTH_NUM + y]->getTag());

	if (!isFinding) {
		if (connectedGems.size() < 3) {
			connectedGems.clear();
			isDisplayingHint = false;
		}
		else {
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

/*
	��ź ���� �� ó��
	int x : ���� �ε���
	int y : ���� �ε���
	int hStart : �������� ������ ��ź�� ������
	int vStart : �������� ������ ��ź�� ������
*/
void GameScene::activatePowerUp(int x, int y, int hStart, int vStart) {
	isRemoved = false;

	if (board[x * WIDTH_NUM + y]->getChildByTag(6) != NULL) {
		board[x * WIDTH_NUM + y]->removeChildByTag(6, true);
	}

	// ��ź ���� �� ����Ʈ
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

	// ���ʴ�� ������ �����ٰ� �ٸ� ��ź�� ������ ����Լ� ȣ��
	for (int i = vStart; i < HEIGHT_NUM; i++) {
		if ((board[i * WIDTH_NUM + y]->getTag() == 5) && (i != x)) {
			activatePowerUp(i, y, 0, i);
		}
		else {
			this->removeChild(board[i * WIDTH_NUM + y], false);
			board[i * WIDTH_NUM + y]->setTag(-1);
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
			}
		}
	}

	idxPup = nullptr;
	isRemoved = true;
}

/*
	�ݹ� �׼� ������ ����Ǵ� �Լ�(�ַ� �ִϸ��̼�)
*/
void GameScene::doRemoveFromParent(Ref* pSender) {
	this->removeChild((Sprite*)pSender);
}

/*
	������ ���̿� ��¦�Ÿ��� ����Ʈ
*/
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

/*
	���� ������Ʈ
	int score : �߰��� ����
*/
void GameScene::addScore(int score) {
	char str[10] = { 0 };

	score_ += score;

	sprintf(str, "%d", score_);
	pScore->setString(str);
}

void GameScene::createShimmer() {
	char str[50] = { 0 };

	for (int i = 0; i < 2; i++) {
		sprintf(str, "gamescene/shimmer/resources-auto/bg-shimmer-%d.png", i);
		log(str);
		auto pShimmer = Sprite::create(str);
		pShimmer->setPosition(Vec2(
			WIDTH_NUM * 40 / 2, 
			HEIGHT_NUM * 40 * rand() % 10));
		pShimmer->setRotation(rand() % 10 * 0.1 * 180 - 90);
		pShimmer->setScale(2);
		pShimmer->setTag(20);
		this->addChild(pShimmer, 2);
		
		pShimmer->runAction(
			RepeatForever::create(
				Sequence::create(
					EaseInOut::create(
						RotateTo::create(rand() % 10 + 5, rand() % 10 * 0.1 * 180 - 90), 2),
					EaseInOut::create(
						MoveTo::create(
							rand() % 10 + 5,
							Vec2(WIDTH_NUM * 40 / 2, HEIGHT_NUM * 40 * rand() % 10 * 0.1)), 2),
					NULL)));
	}
}