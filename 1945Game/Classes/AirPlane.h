#ifndef __AIRPLANE_H__
#define __AIRPLANE_H__

#include "Missile.h"
#include "cocos2d.h"
#include <cstdlib>

USING_NS_CC;

#define RANDOM_INT(MIN, MAX) ((MIN) + rand() % ((MAX + 1) - (MIN)))
#define MAX_MISSILE		2
 

class Missile;

class AirPlane : public cocos2d::Node
{
private:
	Sprite* obj_;

	// 발사된 미사일 객체가 담겨있는 벡터
	Vector<Missile*> missile_;
	
	// 비행기의 타입 (1 : 사용자 2 : 적)
	int planeType_; 
public:
	static AirPlane* create(int planeType);
	bool init(int planeType);
	static void remove(AirPlane* plane);

	// 비행기 생성 후 move 액션
	void moveAction();
	// 비행기가 화면을 벗어났는지 체크
	bool isSceneOut(); 
	// 비행기끼리 충돌 여부 체크
	bool isCrushPlaneToPlane(AirPlane* plane); 
	// 미사일이나 비행기 충돌 시 이펙트
	void putCrashEffect(); 
	// 미사일 발사
	void shooting();

	Sprite* getObj();
	int getPlaneType();
	
	Vector<Missile*> getMissile_();
};

#endif // __AIRPLANE_H__