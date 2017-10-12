//
//  Tile.h
//  SliderPuzzle
//
//  Created by Jaewhan Lee on 13. 1. 26..
//
//

#ifndef __SliderPuzzle__Tile__
#define __SliderPuzzle__Tile__

#include "cocos2d.h"

class Tile : public cocos2d::Sprite
{
public:
    cocos2d::Vec2 originalPosition;
    cocos2d::Vec2 currentPosition;
};

#endif /* defined(__SliderPuzzle__Tile__) */
