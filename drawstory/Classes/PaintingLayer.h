//
//  PaintingLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PaintingLayer_h
#define drawstory_PaintingLayer_h

#include "cocos2d.h"
#include "Brush.h"
#include "PaintCommand.h"


namespace PaintingScene {
    

class CommandBarLayer;
class CanvasLayer;
class PaintingLayer : public cocos2d::CCLayer{
public:
    LAYER_NODE_FUNC(PaintingLayer)
    PaintingLayer();
    virtual ~PaintingLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
//    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
//    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
//    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
protected:    
};
    
    
}

#endif
