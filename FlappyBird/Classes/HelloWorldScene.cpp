#include "HelloWorldScene.h"
#include <cstdlib>
#include <ctime>

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

	/////////////////////////////////
	winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("bird1.png", Rect(0, 0, 86, 60)), "bird1.png");
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("bird2.png", Rect(0, 0, 86, 60)), "bird2.png");
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("bird3.png", Rect(0, 0, 86, 60)), "bird3.png");
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("holdback1.png", Rect(0, 0, 148, 430)), "pipe1.png");
	SpriteFrameCache::getInstance()->addSpriteFrame(
		SpriteFrame::create("holdback2.png", Rect(0, 430, 148, 400)), "pipe2.png");

	birdAnimation = Animation::create();
	birdAnimation->setDelayPerUnit(0.1f);
	birdAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
	birdAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
	birdAnimation->addSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
	birdAnimation->retain();
	
	if (this->createBox2dWorld(true)) {
	_world->SetContactListener((b2ContactListener *)this);

		srand((int)time(nullptr));
		this->setBox2dWorld();
		this->schedule(schedule_selector(HelloWorld::tick));
	}

	return true;
}

bool HelloWorld::createBox2dWorld(bool debug) {
	////////////////////////// 월드 생성 시작

	//중력의 방향 결정
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	// 월드 생성
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	// 디버그 드로잉 설정
	if (debug) {
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);
	}

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// 아래
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width * 5 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 위쪽
	groundEdge.Set(
		b2Vec2(0, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//// 오른쪽
	groundEdge.Set(
		b2Vec2(winSize.width * 5 / PTM_RATIO, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width * 5/ PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	///////////////////////////// 월드 생성 끝

	return true;
}

void HelloWorld::setBox2dWorld() {
	playerIsFlying = false;

	while (bgVector.size() < 2) {
		bgVector.pushBack(Sprite::create("bg.png"));
		bgVector.back()->setAnchorPoint(Vec2::ZERO);
		this->addChild(bgVector.back());
	}

	bgVector.back()->setPositionX(winSize.width);

	bird = this->addNewSprite(Vec2(100, winSize.height/2), Size(86, 60), b2_dynamicBody, "bird1.png", 1);
	Sprite* birdSprite = (Sprite*)bird->GetUserData();

	birdSprite->runAction(RepeatForever::create(Animate::create(birdAnimation)));

	this->runAction(Follow::create(birdSprite));

	pipe1 = this->addNewSprite(Vec2(500, 200), Size(150, 430), b2_staticBody, "pipe1.png", 0);
	pipe2 = this->addNewSprite(Vec2(500, 1200), Size(150, 430), b2_staticBody, "pipe2.png", 0);
	
	//for (int i = 0; i < 2; i++) {
	//	int randx = rand() % (int)winSize.width;
	//	int randy = rand() % (int)winSize.height;

	//	log("randx : %d, randy : %d", randx, randy);
	//	pipe1 = this->addNewSprite(Vec2(randx, randy), Size(150, 830), b2_staticBody, "pipe1.png", 0);
	//	pipe2 = this->addNewSprite(Vec2(randx, randy + 100), Size(150, 830), b2_staticBody, "pipe2.png", 0);
	//}
}

HelloWorld::~HelloWorld() {
	delete _world;
	_world = NULL;
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit() {
	_eventDispatcher->removeEventListener(listener);
	Scene::onExit();
}

void HelloWorld::tick(float dt) {
	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션 한다
	_world->Step(dt, velocityIterations, positionIterations);

	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			Sprite* spriteData = (Sprite*)b->GetUserData();
			spriteData->setPosition(Vec2(
				b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	if (playerIsFlying) {
		bird->ApplyLinearImpulse(
			b2Vec2(0.1, playerVelocity),
			bird->GetWorldCenter(),
			true);
		playerVelocity += 0.1f;

		if (playerVelocity > 1.5f)
			playerVelocity = 1.5f;

	}

	log("bird pos : %f, PTM_RATIO.width : %f", bird->GetPosition().x, winSize.width/ PTM_RATIO);
	if (((int)bird->GetPosition().x % (int)(winSize.width / PTM_RATIO)) == 0) {
		int preIdx = (bgIdx == 0) ? 1 : 0;
		bgVector.at(bgIdx)->setPosition(Vec2(bird->GetPosition().x * PTM_RATIO, 0));

		if ((int)bird->GetPosition().x == (int) winSize.width / PTM_RATIO * 5) {
			log("돌아감");
			bird->SetTransform(b2Vec2(50 / PTM_RATIO, 1280 / 2 / PTM_RATIO), 0.f);
		}
	}
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);

	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

b2Body* HelloWorld::addNewSprite(Vec2 point, Size size, b2BodyType bodytype,
	const char* spriteName, int type) {
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	Sprite* sprite = Sprite::createWithSpriteFrameName(spriteName);
	sprite->setPosition(point);
	this->addChild(sprite);
	bodyDef.userData = sprite;

	b2Body* body = _world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	b2CircleShape circle;
	b2PolygonShape dynamicBox;

	if (type == 0) {
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);
		fixtureDef.shape = &dynamicBox;
	}
	else {
		circle.m_radius = 0.6f;
		fixtureDef.shape = &circle;
	}

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.f;

	body->CreateFixture(&fixtureDef);

	return body;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
	playerVelocity = 0.5f;
	playerIsFlying = true;

	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
	playerIsFlying = false;
	playerVelocity = 0.f;
}

void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void HelloWorld::BeginContact(b2Contact* contact) {
	num = 0;
	log("Contact : Begin");
}

void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	log("Contact : PreSolve");
	log("넌 죽어있다");
	// setEnable 여기서
}