#include "HelloWorldScene.h"
#include "AudioEngine.h"
#include <ctime>

USING_NS_CC;
using namespace experimental;

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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////////////////////
	scene_ = this;
	size_ = this->getContentSize();

	srand(time(NULL));

	auto pLabel = LabelTTF::create("1945", "Thonburi", 34);
	auto pSprite = Sprite::create("Images/bg.png");
	pScore = LabelTTF::create("0 ��", "Thonburi", 20);
	pLife = LabelTTF::create("Life : 3", "Thonburi", 20);

	pLabel->setColor(Color3B::RED);
	pScore->setColor(Color3B::RED);
	pLife->setColor(Color3B::RED);
	pLabel->setPosition(Vec2(size_.width / 2, size_.height - 20));
	pSprite->setPosition(Vec2(size_.width / 2, size_.height / 2));
	pScore->setPosition(Vec2(size_.width - 40, size_.height - 20));
	pLife->setPosition(Vec2(40, size_.height - 20));

	this->addChild(pLabel, 1);
	this->addChild(pSprite, 0);
	this->addChild(pScore, 1);
	this->addChild(pLife, 1);

	scheduleOnce(schedule_selector(HelloWorld::CreatePlayer), 0);
	schedule(schedule_selector(HelloWorld::Update));
	schedule(schedule_selector(HelloWorld::RegenEnemy), 2.5f);
	schedule(schedule_selector(HelloWorld::Shooting), 0.2f);

	AudioEngine::preload("Sounds/explodeEffect.mp3");
	AudioEngine::preload("Sounds/fireEffect.mp3");
	AudioEngine::preload("Sounds/shipDestroyEffect.mp3");
	AudioEngine::play2d("Sounds/mainMainMusic.mp3", true, 1.0f);

    return true;
}

/*
	�÷��̾� ����
*/
void HelloWorld::CreatePlayer(float time = 0) {
	player_ = AirPlane::create(1);
}

void HelloWorld::Update(float time) {
	if (life == 0) { 
		auto pLabel = LabelTTF::create("GAME OVER", "Thonburi", 34);
		pLabel->setColor(Color3B::RED);
		pLabel->setPosition(Vec2(size_.width / 2, size_.height / 2));
		this->addChild(pLabel, 1);
		
		unschedule(schedule_selector(HelloWorld::Update));
		unschedule(schedule_selector(HelloWorld::RegenEnemy));
		unschedule(schedule_selector(HelloWorld::Shooting));
	}

	// ���� �� �����, �̻��� ���� ����
	Vector<AirPlane*> eraseEnemy;
	Vector<Missile*> eraseMissile;

	// �� ����� ���� üũ
	for (auto enemy : enemy_) {
		// ����Ⱑ ȭ���� �������
		if (enemy->isSceneOut()) {
			eraseEnemy.pushBack(enemy);
			continue;
		}

		if (player_) {
			// ������ ����� ������ �浹 ����
			if (enemy->isCrushPlaneToPlane(player_)) {
				eraseEnemy.pushBack(enemy);
				AirPlane::remove(player_);
				player_ = NULL;
				regenCheckTime_ = 0.f;
			}
		}

		for (auto missile : enemy->getMissile_()) {
			// �̻����� ȭ���� �������
			if (missile->isSceneOut()) {
				eraseMissile.pushBack(missile);
				continue;
			}

			if (player_) {
				// �̻��ϰ� ����� ����� �浹 ����
				if (missile->isCrushMissileToPlane(player_)) {
					eraseMissile.pushBack(missile);
					AirPlane::remove(player_);
					player_ = NULL;
					regenCheckTime_ = 0.f;
				}
			}	
		}
	}

	if (player_) {
		// ����� ����� ���� üũ 
		for (auto missile : player_->getMissile_()) {
			// �̻����� ȭ���� �������
			if (missile->isSceneOut()) {
				eraseMissile.pushBack(missile);
				continue;
			}

			for (auto enemy : enemy_) {
				// �̻��ϰ� �� ����� �浹 ����
				if (missile->isCrushMissileToPlane(enemy)) {
					char buf[20] = { 0 };

					score = score + 100;
					sprintf(buf, "%d ��", score);
					pScore->setString(buf);

					eraseMissile.pushBack(missile);
					eraseEnemy.pushBack(enemy);
				}
			}
		}
	}

	// �� ������ �̻����� �����ش�
	for (auto enemy : eraseEnemy) {
		enemy_.eraseObject(enemy);
		AirPlane::remove(enemy);
	}

	for (auto missile : eraseMissile) {
		Missile::remove(missile);
	}

	eraseEnemy.clear();
	eraseMissile.clear();

	//���� ����Ⱑ �׾������� üũ�Ͽ� ��Ȱó��
	if (!player_ && (regenCheckTime_ != -1.f)) {
		regenCheckTime_ += time;
		if (regenCheckTime_ >= 3.f) {
			CreatePlayer();
			life--;

			char buf[20] = { 0 };

			sprintf(buf, "Life : %d", life);
			pLife->setString(buf);
		}
	}
}

/*
	�� ����� ����
	float time : ���� �ð�
*/
void HelloWorld::RegenEnemy(float time) {
	for (int a = 0; a < RANDOM_INT(1, 5); ++a) {
		auto enemy = AirPlane::create(2);
		enemy->moveAction();
		enemy_.pushBack(enemy);
	}
}

/*
	����� �̻��� �߻�
*/
void HelloWorld::Shooting(float time)
{
	for (auto enemy : enemy_) {
		enemy->shooting();	
	}

	if (player_) {
		player_->shooting();
	}
}

void HelloWorld::onEnter() {
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!player_)
		return true;

	auto touchPoint = touch->getLocation();
	auto playerPos = player_->getObj()->getPosition();

	distance_ = Vec2(playerPos.x - touchPoint.x, playerPos.y - touchPoint.y);

	return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!player_)
		return;

	auto touchPoint = touch->getLocation();
	auto pos = Vec2(touchPoint.x + distance_.x, touchPoint.y + distance_.y);

	if (pos.x < 0.f)
	{
		pos.x = 0.f;
		distance_ = Vec2(pos.x - touchPoint.x, pos.y - touchPoint.y);
	}
	else if (pos.x > size_.width)
	{
		pos.x = size_.width;
		distance_ = Vec2(pos.x - touchPoint.x, pos.y - touchPoint.y);
	}

	if (pos.y < 0.f)
	{
		pos.y = 0.f;
		distance_ = Vec2(pos.x - touchPoint.x, pos.y - touchPoint.y);
	}
	else if (pos.y > size_.height)
	{
		pos.y = size_.height;
		distance_ = Vec2(pos.x - touchPoint.x, pos.y - touchPoint.y);
	}

	player_->getObj()->setPosition(pos);
}

Scene* HelloWorld::scene_;
Size HelloWorld::size_;