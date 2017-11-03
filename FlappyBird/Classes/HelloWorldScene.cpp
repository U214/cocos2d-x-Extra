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

	/////////////////////////////////

	winSize = Director::getInstance()->getWinSize();

	// 월드 생성
	if (this->createBox2dWorld(true)) {
		srand((int)time(NULL));

		_world->SetContactListener((b2ContactListener *)this);
		this->setBox2dWorld();
		this->schedule(schedule_selector(HelloWorld::tick));
	}

	return true;
}

bool HelloWorld::createBox2dWorld(bool debug) {
	/////////////// 월드 생성 시작
	// 중력의 방향을 결정한다.
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	// 월드를 생성한다.
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	// 디버그 드로잉 설정
	if (debug) {
		// 적색 : 현재 물리 운동 중
		// 회색 :  현재 물리 운동량이 없음
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);
	}

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	b2Body* groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// 480 X 2 * 320

	// 아래쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width * 2 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	// 왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 위쪽
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO), b2Vec2(winSize.width * 2 / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	// 오른쪽
	groundEdge.Set(b2Vec2(winSize.width * 2 / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width * 2 / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	//////////////////// 월드 생성 끝
	return true;
}

void HelloWorld::setBox2dWorld() {
	auto bg1 = Sprite::create("Images/bg.png");
	bg1->setAnchorPoint(Vec2(0, 0));
	bg1->setContentSize(winSize);
	this->addChild(bg1);

	auto bg2 = Sprite::create("Images/bg.png");
	bg2->setPosition(Vec2(winSize.width, 0));
	bg2->setAnchorPoint(Vec2(0, 0));
	bg2->setContentSize(winSize);
	this->addChild(bg2);

	isFlying = false;

	birdSprite = Sprite::create("Images/bird2.png");
	pipe1Sprite = Sprite::create("Images/holdback1.png");
	pipe2Sprite = Sprite::create("Images/holdback2.png");

	bird = addNewSprite(Vec2(50, 160), birdSprite, b2_dynamicBody, 0);
	pipe1 = addNewSprite(Vec2(300, 0), pipe1Sprite, b2_staticBody, 1);
	pipe2 = addNewSprite(Vec2(300, winSize.height), pipe2Sprite, b2_staticBody, 1);

}

HelloWorld::~HelloWorld() {
	// 월드를 new로 생성했으므로 소멸자에서 지워줌
	delete _world;
	_world = nullptr;
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::tick(float dt) {
	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션한다.
	_world->Step(dt, velocityIterations, positionIterations);

	// 만들어진 객체만큼 루프를 돌리면서 바디에 붙은 스프라이트를 제어한다.
	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != nullptr) {
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(Vec2(
				b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	if (isFlying) {
		bird->ApplyLinearImpulse(b2Vec2(0.03f, birdVelocity),
			bird->GetWorldCenter(),
			true);

		birdVelocity += 0.01f;

		if (birdVelocity > 1.5f) {
			birdVelocity = 1.5f;
		}
	}

	// 새가 화면의 최대 너비에 다다르면 새를 원점에 놓기(파이프도 재설정)
	if (bird->GetPosition().x >= winSize.width * 2 / PTM_RATIO - 1) {
		bird->SetTransform(b2Vec2(0, bird->GetPosition().y), 0);

		pipe1->SetTransform(b2Vec2(
			(10 - (rand() % 3)), rand() % 2), 0);
		pipe2->SetTransform(b2Vec2(
			pipe1->GetPosition().x, winSize.height / PTM_RATIO - rand() % 2), 0);
	}

	// 새가 파이프를 지나면 파이프 위치 재설정
	if (bird->GetPosition().x > pipe1->GetPosition().x + 8) {
		pipe1->SetTransform(b2Vec2(
			(pipe1->GetPosition().x + 17 - (rand() % 3)), rand() % 2), 0);
		
		pipe2->SetTransform(b2Vec2(
			pipe1->GetPosition().x, winSize.height / PTM_RATIO - rand() % 2), 0);
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
	if (!isGameOver) {
		birdVelocity = 0.5f;
		isFlying = true;

		return true;
	}
	else {
		return false;
	}
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
	isFlying = false;
	birdVelocity = 0.0f;
}

b2Body* HelloWorld::addNewSprite(Vec2 location, Sprite* sprite, b2BodyType bodyType, int type) {
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);

	sprite->setScale(0.5f);
	sprite->setPosition(location);
	this->addChild(sprite);

	// 파이프(너무 길어서 사이즈 조정)
	if (type == 1) {
		sprite->setContentSize(Size(
			sprite->getContentSize().width,
			sprite->getContentSize().height / 2));
	}

	bodyDef.userData = sprite;

	b2Body* body = _world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;

	dynamicBox.SetAsBox(
		sprite->getContentSize().width / 4 / PTM_RATIO,
		sprite->getContentSize().height / 4 / PTM_RATIO);
	fixtureDef.shape = &dynamicBox;

	if (type == 0) { // 새
		//this->runAction(Follow::create(sprite));

		fixtureDef.density = 0.5f;
		fixtureDef.friction = 0.5f;
		fixtureDef.restitution = 0.f;
	}

	body->CreateFixture(&fixtureDef);

	return body;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
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
}

void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();

	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();

	if (((bodyA->GetUserData() == birdSprite) && (bodyB->GetUserData() == pipe1Sprite))
		|| ((bodyA->GetUserData() == birdSprite) && (bodyB->GetUserData() == pipe2Sprite))) {
		isGameOver = true;
		
		Sprite* pipeSprite = (Sprite *)bodyB->GetUserData();

		auto gameover = Sprite::create("Images/gameover.png");
		gameover->setPosition(Vec2(pipeSprite->getPosition().x, winSize.height / 2));
		this->addChild(gameover);
	}
}