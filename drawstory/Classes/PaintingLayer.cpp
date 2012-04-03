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
#include "CommandBarLayer.h"
#include "ToggleButton.h"



using namespace cocos2d;



// Z Value
enum { 
    kZCanvasLayer = 1,
    kZCommandBar = 2,
};


// Tags
enum { 
    kTagCanvasLayer = 100,
    kTagCommandBarLayer,
};

PaintingLayer::PaintingLayer() {
}

PaintingLayer::~PaintingLayer() {
}

bool PaintingLayer::init(){
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite* title = CCSprite::spriteWithSpriteFrameName("pl_background_title");
        CC_BREAK_IF(!title);
        title->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height - (title->getContentSize().height * 0.5f)));
        addChild(title);

        CCPoint canvasPosition = CCPointMake(0, 57.0f);
        CanvasLayer* canvas = CanvasLayer::node();
        CC_BREAK_IF(!canvas);
        canvas->setPosition(canvasPosition);
        addChild(canvas,kZCanvasLayer,kTagCanvasLayer);
        
        CommandBarLayer* cmdbar = CommandBarLayer::node();
        CC_BREAK_IF(!cmdbar);
        addChild(cmdbar,kZCommandBar,kTagCommandBarLayer);
        cmdbar->setCanvasLayer(canvas);
        
    
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

