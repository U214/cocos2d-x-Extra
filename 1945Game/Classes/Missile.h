#ifndef __MISSILE_H__
#define __MISSILE_H__

#include "AirPlane.h"
#include "cocos2d.h"

USING_NS_CC;

class AirPlane;

class Missile : public cocos2d::Node
{
private:
	Sprite* obj_;
	AirPlane* plane_;

public:
	static Missile* create(Vec2& pos, AirPlane* plane);
	bool init(Vec2& pos, AirPlane* plane);
	static void remove(Missile* missile);

	// �̻��� ���� �� move �׼�
	void moveAction();
	// �̻����� ȭ���� ������� üũ
	bool isSceneOut(); 
	// �̻��ϰ� ������� �浹 ���� üũ
	bool isCrushMissileToPlane(AirPlane* plane); 
	
	Sprite* getObj();
	AirPlane* getPlane();
};

#endif // __MISSILE_H__