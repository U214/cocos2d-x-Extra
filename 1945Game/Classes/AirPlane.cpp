#include "AirPlane.h"
#include "HelloWorldScene.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

AirPlane* AirPlane::create(int planeType) {
	AirPlane* plane = new AirPlane();

	if (plane && plane->init(planeType)) {
		return plane;
	}
	CC_SAFE_DELETE(plane);
	return nullptr;
}

bool AirPlane::init(int planeType) {
	planeType_ = planeType;
	
	if (planeType_ == 1) { // ����� �����
		obj_ = Sprite::create("Images/airplain_01.png");
	}
	else if (planeType_ == 2) { // �� �����
		obj_ = Sprite::create("Images/enemy_01.png");
	}

	if (obj_) {
		if (planeType_ == 1) {
			obj_->setPosition(Vec2(HelloWorld::size_.width / 2, 50));
		}
		else if (planeType_ == 2) {
			obj_->setPosition(Vec2(RANDOM_INT(0, (int)HelloWorld::size_.width), HelloWorld::size_.height));
		}
		HelloWorld::scene_->addChild(obj_);

		return true;
	}
	return false;
}

void AirPlane::remove(AirPlane* plane) {
	HelloWorld::scene_->removeChild(plane->getObj());

	if (plane->getPlaneType() == 2) {
		delete plane;
	}
}

/*
	����Ⱑ ȭ���� ������� üũ(�� ����⿡ �ش�)
*/
bool AirPlane::isSceneOut() { 
	if (obj_->getPosition().y <= 0) {
		return true;
	}
	return false;
}

/*
	����Ⳣ�� �浹 ���� üũ
	AirPlane* plane : �浹�� �����
*/
bool AirPlane::isCrushPlaneToPlane(AirPlane* plane) {
	if (obj_->getBoundingBox().intersectsRect(plane->getObj()->getBoundingBox())) {
		AudioEngine::play2d("Sounds/shipDestroyEffect.mp3", false, 1.0f);

		putCrashEffect();
		plane->putCrashEffect();

		return true;
	}
	return false;
}

/*
	�̻����̳� ����� �浹 �� ����Ʈ
*/
void AirPlane::putCrashEffect() {						  
	auto expBase = Sprite::create("Images/exp_01.png");
	expBase->setPosition(obj_->getPosition());

	auto animation = Animation::create();
	animation->addSpriteFrameWithFileName("Images/exp_01.png");
	animation->addSpriteFrameWithFileName("Images/exp_02.png");
	animation->addSpriteFrameWithFileName("Images/exp_03.png");
	animation->addSpriteFrameWithFileName("Images/exp_04.png");
	animation->addSpriteFrameWithFileName("Images/exp_05.png");

	animation->setDelayPerUnit(0.1f);

	animation->setRestoreOriginalFrame(true);

	auto animate = Animate::create(animation);
	auto removeAction = CallFunc::create(CC_CALLBACK_0(Node::removeChild, HelloWorld::scene_, expBase, true));

	auto seq = Sequence::create(animate, removeAction, NULL);
	HelloWorld::scene_->addChild(expBase);

	AudioEngine::play2d("Sounds/explodeEffect.mp3", false, 1.0f);

	expBase->runAction(seq);
}

/*
	�̻��� �߻�(Missile ��ü ����)
*/
void AirPlane::shooting() { 
	if (planeType_ == 1) {
		AudioEngine::play2d("Sounds/fireEffect.mp3", false, 1.0f);

		missile_.pushBack(Missile::create(
			Vec2(obj_->getPosition().x - 16.f, obj_->getPosition().y),
			this));
		missile_.back()->moveAction();

		missile_.pushBack(Missile::create(
			Vec2(obj_->getPosition().x + 16.f, obj_->getPosition().y),
			this));
		missile_.back()->moveAction();
	}
	else if (planeType_ == 2) {
		missile_.pushBack(Missile::create(
			Vec2(obj_->getPosition().x, obj_->getPosition().y),
			this));
		missile_.back()->moveAction();
	}
}

/*
	����� ��������Ʈ ��ü(obj_) ����
*/
Sprite* AirPlane::getObj() {
	return obj_;
}

/*
	������� Ÿ��(planeType_) ����
	(1 : �����, 2 : ��)
*/
int AirPlane::getPlaneType() {
	return planeType_;
}

/*
	�߻�� �̻��� ��ü�� ����ִ� ����(missile_) ����
*/
Vector<Missile*> AirPlane::getMissile_() {
	return missile_;
}

/*
	����� ���� �� move �׼�(�� ����⿡ �ش�)
*/
void AirPlane::moveAction() {
	obj_->runAction(MoveBy::create(3.f, Vec2(0, -HelloWorld::size_.height - 30)));
}
