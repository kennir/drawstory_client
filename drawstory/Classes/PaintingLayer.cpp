//
//  PaintingLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintingLayer.h"
#include "types.h"
#include "CanvasLayer.h"




using namespace cocos2d;


enum { kZCanvasLayer = 1 };
enum { kTagCanvasLayer = 1000 };

PaintingLayer::PaintingLayer() : canvas_(NULL) { 
}

PaintingLayer::~PaintingLayer() {
    CC_SAFE_RELEASE(canvas_);
}

bool PaintingLayer::init(){
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        canvas_ = CanvasLayer::node();
        CC_BREAK_IF(!canvas_);
        canvas_->retain();
        addChild(canvas_,kZCanvasLayer,kTagCanvasLayer);
        
    
        result = true;
    } while (0);
    return result;
}

void PaintingLayer::onEnter() {
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityPaintingLayer, true);
}

void PaintingLayer::onExit() {
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool PaintingLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
    return true;
}

void PaintingLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
}

void PaintingLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
}

void PaintingLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
}

