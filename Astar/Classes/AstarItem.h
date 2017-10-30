#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class AstarItem : public Node
{
public:
	cocos2d::Vec2 mPos;   // 좌표
	int g;            // 시작점으로부터의 거리
	int h;            // 목표점까지의 거리
	int f;            // 거리의 합
	cocos2d::Vec2 pPos;   // 부모좌표

	AstarItem() {
		g = 0;
		h = 0;
		f = 0;
	}

	AstarItem(Vec2 mPos_, Vec2 pPos_, int g_, int h_) {
		mPos = mPos_;
		pPos = pPos_;
		g = g_;
		h = h_;
		f = g_ + h_;
	}
};