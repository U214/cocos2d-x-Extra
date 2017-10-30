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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////////

	winSize = Director::getInstance()->getWinSize();

	// 게임 블럭 화면 중앙으로 옮기기
	board = LayerColor::create(Color4B::WHITE);
	board->setContentSize(Size(MAX_X * SIZE, MAX_Y * SIZE));
	board->setTag(10);
	board->setPosition(Vec2(winSize.width / 2 - MAX_X * SIZE / 2,
		winSize.height / 2 - MAX_Y * SIZE / 2));
	this->addChild(board);

	// 가상의 구역 설정 (선긋기)
	drawLines();

	drawMap();

	openPath = new std::vector<AstarItem*>();
	closePath = new std::vector<AstarItem*>();
	myPath = new std::vector<Vec2>();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HelloWorld::drawLines() {
	auto draw_node = DrawNode::create();
	board->addChild(draw_node);

	// draw line : Y
	for (int i = 1; i <= MAX_Y; i++) {
		draw_node->drawLine(
			Vec2(0, SIZE * i),
			Vec2(MAX_X * SIZE, SIZE * i),
			Color4F(0, 0, 0, 0.2));
	}

	// draw line : X
	for (int i = 1; i <= MAX_X; i++) {
		draw_node->drawLine(
			Vec2(SIZE * i, 0),
			Vec2(SIZE * i, MAX_X * SIZE),
			Color4F(0, 0, 0, 0.2));
	}
}

void HelloWorld::drawMap() {
	//0000000,
	//0000000,
	//000X000,
	//0A0X0Z0,
	//000X000,
	//0000000,
	//0000000,

	string s = "00000000000000000X0000A0X0Z0000X00000000000000000";

	for (int j = 0; j < MAX_Y; j++) {
		for (int i = 0; i < MAX_X; i++) {
			arrCanvas[i][j] = (char)s[i + j*MAX_Y];
			//log("%d...%d...%c", i, j, arrCanvas[i][j]);
		}
	}

	// 화면에 그리기 시작
	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			char tChar = arrCanvas[i][j];

			if (tChar == '0') {

			}
			else if (tChar == 'A') {
				// log("%d...%d", i, j
				tamara = Sprite::create("grossinis_sister1.png");
				log("create sister pos %d, %d", i, j);
				tamara->setPosition(Vec2(i*SIZE + SIZE / 2, j*SIZE + SIZE / 2));
				tamara->setScale(0.3f);

				board->addChild(tamara);
			}
			else if (tChar == 'X') {
				// log("%d...%d", i, j
				auto tImg = Sprite::create("rect.png");
				tImg->setPosition(Vec2(i * SIZE + SIZE / 2, j * SIZE + SIZE / 2));
				board->addChild(tImg);
			}
		}
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
	pCoordS = Vec2(
		(tamara->getPosition().x - SIZE / 2) / SIZE,
		(tamara->getPosition().y - SIZE / 2) / SIZE);

	pCoordE = CoordForPosition(touch->getLocation());
	
	// 화면 밖이나 장애물 클릭 할 경우
	if ((arrCanvas[(int)pCoordE.x][(int)pCoordE.y] == 'X') ||
		(pCoordE.x < 0) || (pCoordE.y < 0) || 
		(pCoordE.x > 6) || (pCoordE.y > 6)) return false;

	for (int i = 0; i < openPath->size(); i++) {
		auto item = (AstarItem*)openPath->at(i);
		delete item;
	}

	openPath->clear();

	for (int i = 0; i < closePath->size(); i++) {
		auto item = (AstarItem*)closePath->at(i);
		delete item;
	}

	closePath->clear();

	myPath->clear();

	AStartPathFind();
	findPath();
	movePath();

	return true;
}

void HelloWorld::AStartPathFind() {
	closePath->push_back(new AstarItem(
		pCoordS,
		pCoordS,
		0,
		getDistance(pCoordS, pCoordE, 1)));

	while (1) {
		AstarItem* midItem = closePath->back();

		for (int i = midItem->mPos.x - 1; i <= midItem->mPos.x + 1; i++) {
			for (int j = midItem->mPos.y - 1; j <= midItem->mPos.y + 1; j++) {

				isRoad(
					Vec2(i, j),
					midItem->mPos,
					midItem->g,
					getDistance(midItem->mPos, Vec2(i, j), 0));
			}
		}

		for (int i = 0; i < openPath->size(); i++) {
			if (openPath->at(i)->mPos == pCoordE) return;
		}

		int minIdx = 0;
		int minFCost = openPath->at(0)->f;

		for (int i = 1; i < openPath->size(); i++) {
			if (minFCost > openPath->at(i)->f) {
				minIdx = i;
				minFCost = openPath->at(i)->f;
			}
		}

		closePath->push_back(openPath->at(minIdx));
		openPath->erase(openPath->begin() + minIdx);
	}
}

void HelloWorld::isRoad(Vec2 mPos, Vec2 pPos, int pGCost, int mGCost) {
	if ((mPos.x < 0) || (mPos.y < 0) || (mPos.x > 6) || (mPos.y > 6)) return;

	// openPath에 있는지 확인
	for (int i = 0; i < openPath->size(); i++) {
		if (mPos == openPath->at(i)->mPos) return;
	}

	// closePath에 있는지 확인
	for (int i = 0; i < closePath->size(); i++) {
		if (mPos == closePath->at(i)->mPos) return;
	}

	// 장애물 여부
	if (arrCanvas[(int)mPos.x][(int)mPos.y] == 'X') return;

	// 대각선 일때 장애물 충돌 X
	if (mGCost == 14) {
		if ((arrCanvas[(int)pPos.x][(int)mPos.y] == 'X') ||
			(arrCanvas[(int)mPos.x][(int)pPos.y] == 'X')) return;
	}

	auto item = new AstarItem(mPos, pPos, pGCost + mGCost, getDistance(pCoordE, mPos, 1));
	openPath->push_back(item);
}

/*
	길 만들기 (myPath push)
*/
void HelloWorld::findPath() {
	myPath->push_back(pCoordE);
	AstarItem* path = closePath->back();

	while (1) {
		myPath->push_back(path->mPos);

		if (path->mPos == pCoordS) break;

		for (int i = 0; i < closePath->size(); i++) {
			if (closePath->at(i)->mPos == path->pPos) {
				path = closePath->at(i);
				break;
			}
		}
	}
}

/*
	만들어진 길로 이동
*/
void HelloWorld::movePath() {
	Vector<FiniteTimeAction*> vact;

	for (int i = myPath->size() - 1; i >= 0; i--) {
		vact.pushBack((MoveTo::create(0.5f, Vec2(
			myPath->at(i).x * SIZE + SIZE / 2,
			myPath->at(i).y * SIZE + SIZE / 2))));
	}
	tamara->runAction(Sequence::create(vact));

	vact.clear();
}

/*
	위치 값 좌표로 변환
*/
Vec2 HelloWorld::CoordForPosition(Vec2 position) {
	float x = (position.x - ((this->getContentSize().width - board->getContentSize().width) / 2)) / SIZE;
	float y = (position.y - ((this->getContentSize().height - board->getContentSize().height) / 2)) / SIZE;

	return Vec2(floor(x), floor(y));
}

/*
	두 점 사이의 거리
	int type : g와 h의 여부 (g - 0, h - 1)
*/
int HelloWorld::getDistance(Vec2 d1, Vec2 d2, int type) {
	if (type == 0) {
		return floor(sqrt((pow(d1.x - d2.x, 2) + pow(d1.y - d2.y, 2))) * 10);
	}
	else {
		return (abs(d1.x - d2.x) + abs(d1.y - d2.y)) * 10;
	}
}