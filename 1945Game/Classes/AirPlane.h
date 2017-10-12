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

	// �߻�� �̻��� ��ü�� ����ִ� ����
	Vector<Missile*> missile_;
	
	// ������� Ÿ�� (1 : ����� 2 : ��)
	int planeType_; 
public:
	static AirPlane* create(int planeType);
	bool init(int planeType);
	static void remove(AirPlane* plane);

	// ����� ���� �� move �׼�
	void moveAction();
	// ����Ⱑ ȭ���� ������� üũ
	bool isSceneOut(); 
	// ����Ⳣ�� �浹 ���� üũ
	bool isCrushPlaneToPlane(AirPlane* plane); 
	// �̻����̳� ����� �浹 �� ����Ʈ
	void putCrashEffect(); 
	// �̻��� �߻�
	void shooting();

	Sprite* getObj();
	int getPlaneType();
	
	Vector<Missile*> getMissile_();
};

#endif // __AIRPLANE_H__