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
	�̻����� ȭ���� ������� üũ
*/
bool Missile::isSceneOut() { 
	bool isSceneOut = false;

	if (plane_->getPlaneType() == 1) { // ����� �����
		isSceneOut = (obj_->getPositionY() >= HelloWorld::size_.height);
	}
	else if (plane_->getPlaneType() == 2) { // �� �����
		isSceneOut = (obj_->getPositionY() <= 0);
	}

	if (isSceneOut) {	
		return true;
	}	
	return false;
}

/*
	�̻��ϰ� ������� �浹 ���� üũ
	AirPlane* plane : �浹�� �����
*/
bool Missile::isCrushMissileToPlane(AirPlane* plane) { 
	if (obj_->getBoundingBox().intersectsRect(plane->getObj()->getBoundingBox())) {
		plane->putCrashEffect();
		return true;
	}
	return false;
}

/*
	�̻��� ���� �� move �׼�
*/
void Missile::moveAction() {
	if (plane_->getPlaneType() == 1) { // ����� ������� �̻���
		obj_->runAction(MoveBy::create(0.75f, Vec2(0, HelloWorld::size_.height + 20)));
	}
	else if (plane_->getPlaneType() == 2) { // �� ������� �̻���
		obj_->runAction(MoveBy::create(0.8f, Vec2(0, -HelloWorld::size_.height - 20)));
	}
}

/*
	�̻��� ��������Ʈ ��ü(obj_) ����
*/
Sprite* Missile::getObj() {
	return obj_;
}

/*
	�̻����� �߻��� ����� ��ü(plane_) ����
*/
AirPlane* Missile::getPlane() {
	return plane_;
}