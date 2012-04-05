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
#include "PS_CommandBarLayer.h"
#include "ToggleButton.h"
#include "PS_TitleBarLayer.h"



using namespace cocos2d;

namespace PaintingScene {



// Z Value
enum { 
    kZCanvasLayer = 1,
    kZCommandBarLayer = 2,
    kZTitleBarLayer = 2,
};


// Tags
enum { 
    kTagCanvasLayer = 100,
    kTagCommandBarLayer,
    kTagTitleBarLayer,
};

PaintingLayer::PaintingLayer() {
}

PaintingLayer::~PaintingLayer() {
}

bool PaintingLayer::init(){
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCPoint canvasPosition = CCPointMake(0, 57.0f);
        CanvasLayer* canvas = CanvasLayer::node();
        CC_BREAK_IF(!canvas);
        canvas->setPosition(canvasPosition);
        addChild(canvas,kZCanvasLayer,kTagCanvasLayer);
        
        TitleBarLayer* titlebar = TitleBarLayer::node();
        CC_BREAK_IF(!titlebar);
        titlebar->setPosition(CCPointMake(0, 57.0f + 333.0f ));
        addChild(titlebar,kZTitleBarLayer,kTagTitleBarLayer);
        titlebar->setCanvasLayer(canvas);
        
        CommandBarLayer* cmdbar = CommandBarLayer::node();
        CC_BREAK_IF(!cmdbar);
        addChild(cmdbar,kZCommandBarLayer,kTagCommandBarLayer);
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
    
    // reset all 
    static_cast<CanvasLayer*>(getChildByTag(kTagCanvasLayer))->resetToInitial();
}

bool PaintingLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    return true;
}
    
    
}

