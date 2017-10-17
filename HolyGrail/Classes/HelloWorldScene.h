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
	
	// 맵 초기화
	void initMap(Ref* pSender);
	// 맵 생성, 객체들 위치 값 받기
	void createMap();
	// 객체 생성
	void createObject();
	// 객체 애니메이션
	void createAnimation();

	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);

	// 메타 레이어에 의해 캐릭터 위치 값 설정(아이템, 몬스터 관련)
	void setPlayerPosition(cocos2d::Vec2 position);

	void onEnter();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	// 캐릭터 이동
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	cocos2d::Texture2D*						texture;
	cocos2d::EventListenerTouchOneByOne*	listener;
	cocos2d::Size							winSize;
	cocos2d::LabelTTF*						stage;		// 화면에 나오는 스테이지 단계
	cocos2d::Layer*							stageLayer;	// 스테이지 레이어
	std::vector<cocos2d::Color4B>			stageColor;	// 스테이지 배경색

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

	// 먹고 먹히는 관계 (K:아이템 V:객체)
	std::map<std::string, std::string>		objectMap;
	// 아이템에 해당되는 Sprite Frame 이름 (K:아이템 V:Sprite Frame이름)
	std::map<std::string, std::string>		itemFrameMap;
	// 객체마다 지정된 태그 값 (K:객체 V:태그번호)
	std::map<std::string, int>				objectTagMap;

	// 현재 가지고 있는 아이템
	std::string haveItem;
	// 현재 스테이지 번호
	int stageNum = 1;
};

#endif // __HELLOWORLD_SCENE_H__
