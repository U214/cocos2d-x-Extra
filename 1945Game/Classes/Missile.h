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

	// 미사일 생성 후 move 액션
	void moveAction();
	// 미사일이 화면을 벗어났는지 체크
	bool isSceneOut(); 
	// 미사일과 비행기의 충돌 여부 체크
	bool isCrushMissileToPlane(AirPlane* plane); 
	
	Sprite* getObj();
	AirPlane* getPlane();
};

#endif // __MISSILE_H__