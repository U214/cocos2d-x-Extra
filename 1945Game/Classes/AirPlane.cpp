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
	
	if (planeType_ == 1) { // 사용자 비행기
		obj_ = Sprite::create("Images/airplain_01.png");
	}
	else if (planeType_ == 2) { // 적 비행기
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
	비행기가 화면을 벗어났는지 체크(적 비행기에 해당)
*/
bool AirPlane::isSceneOut() { 
	if (obj_->getPosition().y <= 0) {
		return true;
	}
	return false;
}

/*
	비행기끼리 충돌 여부 체크
	AirPlane* plane : 충돌한 비행기
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
	미사일이나 비행기 충돌 시 이펙트
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
	미사일 발사(Missile 객체 생성)
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
	비행기 스프라이트 객체(obj_) 리턴
*/
Sprite* AirPlane::getObj() {
	return obj_;
}

/*
	비행기의 타입(planeType_) 리턴
	(1 : 사용자, 2 : 적)
*/
int AirPlane::getPlaneType() {
	return planeType_;
}

/*
	발사된 미사일 객체가 담겨있는 벡터(missile_) 리턴
*/
Vector<Missile*> AirPlane::getMissile_() {
	return missile_;
}

/*
	비행기 생성 후 move 액션(적 비행기에 해당)
*/
void AirPlane::moveAction() {
	obj_->runAction(MoveBy::create(3.f, Vec2(0, -HelloWorld::size_.height - 30)));
}
