#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "CustomTableViewCell.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class HelloWorld
	: public cocos2d::Scene
	, public cocos2d::extension::TableViewDataSource
	, public cocos2d::extension::TableViewDelegate
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	cocos2d::Sprite* background;
	cocos2d::EventListenerTouchAllAtOnce* listener;
	cocos2d::Size winSize;
	cocos2d::RenderTexture* m_pTarget;
	cocos2d::Vector<cocos2d::Sprite*> m_pBrush;
	std::vector<cocos2d::Color3B> colors;

	cocos2d::Color3B curColor = Color3B::RED;
	cocos2d::extension::TableViewCell* preCell;

	~HelloWorld();
	virtual void onEnter();
	virtual void onExit();
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void saveImage(Ref* sender);
	void clearImage(Ref* sender);

	virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {};
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView* table);
};

#endif // __HELLOWORLD_SCENE_H__
