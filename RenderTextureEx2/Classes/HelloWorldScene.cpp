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

	/////////////////////// 크레용 색 벡터에 담기
	colors.push_back(Color3B(255, 3, 2)); // 빨간색
	colors.push_back(Color3B(255, 113, 2)); // 주황색
	colors.push_back(Color3B(255, 219, 0)); // 노란색
	colors.push_back(Color3B(33, 215, 0)); // 연두색
	colors.push_back(Color3B(0, 154, 255)); // 하늘색
	colors.push_back(Color3B(255, 101, 222)); // 분홍색
	colors.push_back(Color3B(181, 0, 255)); // 보라색
	colors.push_back(Color3B::WHITE);
	colors.push_back(Color3B(30, 30, 30)); // 검정색
	colors.push_back(Color3B(156, 154, 156)); // 회색
	colors.push_back(Color3B(115, 65, 0)); // 올리브색
	colors.push_back(Color3B(255, 207, 156)); // 살색
	colors.push_back(Color3B(255, 231, 214)); // 옅은 살색

	/////////////////////////////크레용 테이블
	TableView* tableView = TableView::create(this, Size(320, 100));
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView->setPosition(Vec2(80, 0));
	tableView->setDelegate(this);
	tableView->setTag(100);
	this->addChild(tableView, 2);
	tableView->reloadData();

	///////////////////////////////그리기 화면

	winSize = Director::getInstance()->getWinSize();

	background = Sprite::create("Images/image01.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 40));

	this->addChild(background, 1);

	// Render Texture 만들기
	m_pTarget = RenderTexture::create(winSize.width, winSize.height, Texture2D::PixelFormat::RGBA8888);
	m_pTarget->retain();
	m_pTarget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	this->addChild(m_pTarget, 0);

	MenuItemFont::setFontSize(16);

	auto item1 = MenuItemFont::create("Clear", CC_CALLBACK_1(HelloWorld::clearImage, this));
	item1->setColor(Color3B::BLACK);

	auto menu = Menu::create(item1, NULL);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(winSize.width - 80, winSize.height - 30));

	this->addChild(menu, 2);

	return true;
}

void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell) {
	curColor = colors[cell->getIdx()];

	if (preCell->getIdx() != cell->getIdx()) {
		auto sprite = (Sprite*)cell->getChildByTag(120);
		auto preSprite = (Sprite*)preCell->getChildByTag(120);

		sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y  + 20);
		preSprite->setPosition(preSprite->getPosition().x, preSprite->getPosition().y - 20);
		
		preCell = cell;
	}
}

Size HelloWorld::tableCellSizeForIndex(TableView* table, ssize_t idx) {
	return Size(40, 85);
}

TableViewCell* HelloWorld::tableCellAtIndex(TableView* table, ssize_t idx) {
	auto string = String::createWithFormat("%ld", idx);
	TableViewCell* cell = table->dequeueCell();

	char str[50] = { 0 };
	sprintf(str, "Images/crayon_%02d.png", idx + 1);

	// 화면에 보이는건 if문을 탄다(그 정도 개수를 만들고 난 다음에 돌려쓰기(else)
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();

		auto sprite = Sprite::create(str);
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(0, 0));
		sprite->setScale(0.5f);
		sprite->setTag(120);
		cell->addChild(sprite);
		
		auto label = LabelTTF::create(string->getCString(), "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setColor(Color3B::BLACK);
		label->setTag(121);
		cell->addChild(label);

		if (preCell == NULL) {
			preCell = cell;

			auto sprite = (Sprite*)preCell->getChildByTag(120);
			sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y + 20);
		}
	}
	else {
		auto sprite = (Sprite*)cell->getChildByTag(120);
		sprite->setTexture(str);

		auto label = (LabelTTF*)cell->getChildByTag(121);
		label->setString(string->getCString());
	}

	return cell;
}

ssize_t HelloWorld::numberOfCellsInTableView(TableView* table) {
	return 13;
}

HelloWorld::~HelloWorld() {
	m_pTarget->release();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit() {
	_eventDispatcher->removeEventListener(listener);

	Scene::onExit();
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
	auto touch = touches[0];
	Vec2 start = touch->getLocation();
	Vec2 end = touch->getPreviousLocation();

	m_pTarget->begin();

	float distance = start.getDistance(end);

	if (distance > 1) {
		int d = (int)distance;
		m_pBrush.clear();

		for (int i = 0; i < d; ++i) {
			Sprite* sprite = Sprite::create("Images/brush.png");
			sprite->setColor(curColor);
			m_pBrush.pushBack(sprite);
		}

		for (int i = 0; i < d; i++) {
			float difx = end.x - start.x;
			float dify = end.y - start.y;
			float delta = (float)i / distance;

			if (background->getBoundingBox().containsPoint(
				Vec2(start.x + (difx * delta), start.y + (dify * delta)))) {
				m_pBrush.at(i)->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
				m_pBrush.at(i)->setRotation(rand() % 360);

				float r = (float)(rand() % 50 / 50.f) + 0.25f;
				m_pBrush.at(i)->setScale(r);
				m_pBrush.at(i)->visit();
			}
		}
	}
	m_pTarget->end();
}

void HelloWorld::clearImage(Ref* sender) {
	m_pTarget->clear(255, 255, 255, 0);
}