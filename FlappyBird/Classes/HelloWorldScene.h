#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class HelloWorld 
	: public cocos2d::Scene
	, public b2ContactListener
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	Size winSize;
	Texture2D* texture;
	b2World* _world;

	// For debugging
	GLESDebugDraw* m_debugDraw;
	CustomCommand _customCmd;

	~HelloWorld();
	bool createBox2dWorld(bool debug);
	void setBox2dWorld();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform,
		uint32_t flags) override;
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

	void onEnter();
	void tick(float dt);
	
	b2Body*		pipe1;
	b2Body*		pipe2;
	b2Body*		bird;

	Sprite*		pipe1Sprite;
	Sprite*		pipe2Sprite;
	Sprite*		birdSprite;
	float		birdVelocity;
	bool		isFlying;
	bool		isGameOver = false;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	b2Body* addNewSprite(Vec2 location, Sprite* sprite, b2BodyType bodyType, int type);

	void BeginContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif // __HELLOWORLD_SCENE_H__