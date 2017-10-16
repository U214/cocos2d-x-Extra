#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	void initMap(Ref* pSender);
	void createMap();
	void createObject();
	void createAnimation();

	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	void setPlayerPosition(cocos2d::Vec2 position);

	void onEnter();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	cocos2d::Texture2D*						texture;
	cocos2d::EventListenerTouchOneByOne*	listener;
	cocos2d::Size							winSize;
	cocos2d::LabelTTF*						stage;
	cocos2d::Layer*							stageLayer;
	std::vector<cocos2d::Color4B>			stageColor;

	cocos2d::Vec2							heroPosition;
	cocos2d::Vec2							boxPosition;
	cocos2d::Vec2							keyPosition;
	std::vector<cocos2d::Vec2>				wolfPosition;
	std::vector<cocos2d::Vec2>				jellyPosition;
	std::vector<cocos2d::Vec2>				demonPosition;
	std::vector<cocos2d::Vec2>				swordPosition;
	std::vector<cocos2d::Vec2>				macePosition;
	std::vector<cocos2d::Vec2>				holySwordPosition;

	cocos2d::Sprite*						hero;
	cocos2d::Sprite*						box;
	cocos2d::Sprite*						key;
	cocos2d::Sprite*						itemBox;
	cocos2d::Vector<cocos2d::Sprite*>		wolf;
	cocos2d::Vector<cocos2d::Sprite*>		jelly;
	cocos2d::Vector<cocos2d::Sprite*>		demon;
	cocos2d::Vector<cocos2d::Sprite*>		sword;
	cocos2d::Vector<cocos2d::Sprite*>		mace;
	cocos2d::Vector<cocos2d::Sprite*>		holySword;

	cocos2d::Animation*						heroUpAnimation;
	cocos2d::Animation*						heroDownAnimation;
	cocos2d::Animation*						heroLeftAnimation;
	cocos2d::Animation*						heroRightAnimation;
	cocos2d::Animation*						wolfAnimation;
	cocos2d::Animation*						jellyAnimation;
	cocos2d::Animation*						demonAnimation;

	cocos2d::TMXTiledMap*					tmap;
	cocos2d::TMXLayer*						metainfo;
	cocos2d::TMXLayer*						indexinfo;

	std::map<std::string, std::string>		objectMap;
	std::map<std::string, std::string>		itemFrameMap;
	std::map<std::string, int>				objectTagMap;

	std::string haveItem;
	int stageNum = 1;
};

#endif // __HELLOWORLD_SCENE_H__
