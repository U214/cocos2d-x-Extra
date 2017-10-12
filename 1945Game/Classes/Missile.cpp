#include "Missile.h"
#include "HelloWorldScene.h"

Missile* Missile::create(Vec2& pos, AirPlane* plane) {
	Missile* missile = new Missile();

	if (missile && missile->init(pos, plane)) {
		return missile;
	}
	
	CC_SAFE_DELETE(missile);
	return nullptr;
}

bool Missile::init(Vec2& pos, AirPlane* plane) {
	plane_ = plane;
	obj_ = Sprite::create("Images/missile.png");

	if (obj_) {
		obj_->setPosition(pos);
		HelloWorld::scene_->addChild(obj_);
		
		return true;
	}
	return false;
}

void Missile::remove(Missile* missile) {
	missile->getPlane()->getMissile_().eraseObject(missile);
}

/*
	미사일이 화면을 벗어났는지 체크
*/
bool Missile::isSceneOut() { 
	bool isSceneOut = false;

	if (plane_->getPlaneType() == 1) { // 사용자 비행기
		isSceneOut = (obj_->getPositionY() >= HelloWorld::size_.height);
	}
	else if (plane_->getPlaneType() == 2) { // 적 비행기
		isSceneOut = (obj_->getPositionY() <= 0);
	}

	if (isSceneOut) {	
		return true;
	}	
	return false;
}

/*
	미사일과 비행기의 충돌 여부 체크
	AirPlane* plane : 충돌한 비행기
*/
bool Missile::isCrushMissileToPlane(AirPlane* plane) { 
	if (obj_->getBoundingBox().intersectsRect(plane->getObj()->getBoundingBox())) {
		plane->putCrashEffect();
		return true;
	}
	return false;
}

/*
	미사일 생성 후 move 액션
*/
void Missile::moveAction() {
	if (plane_->getPlaneType() == 1) { // 사용자 비행기의 미사일
		obj_->runAction(MoveBy::create(0.75f, Vec2(0, HelloWorld::size_.height + 20)));
	}
	else if (plane_->getPlaneType() == 2) { // 적 비행기의 미사일
		obj_->runAction(MoveBy::create(0.8f, Vec2(0, -HelloWorld::size_.height - 20)));
	}
}

/*
	미사일 스프라이트 객체(obj_) 리턴
*/
Sprite* Missile::getObj() {
	return obj_;
}

/*
	미사일을 발사한 비행기 객체(plane_) 리턴
*/
AirPlane* Missile::getPlane() {
	return plane_;
}