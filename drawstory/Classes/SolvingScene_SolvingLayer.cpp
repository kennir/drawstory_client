//
//  SolveScene_SolveLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SolvingScene_SolvingLayer.h"
#include "SolvingScene_TitleBarLayer.h"
#include "SolvingScene_SolveKeyboardLayer.h"
#include "PaintingReplayLayer.h"

using namespace cocos2d;


namespace SolvingScene {
    
    
    // Z Value
    enum { 
        kZReplayLayer = 1,
        kZKeyboardLayer = 2,
        kZTitleBarLayer = 2,
    };
    
    
    // Tags
    enum { 
        kTagReplayLayer = 100,
        kTagKeyboardLayer,
        kTagTitleBarLayer,
    };
    
    
    
    bool SolvingLayer::init() {
        bool result = false;
        do {
            CC_BREAK_IF(!CCLayer::init());
            
            
            // title bar
            TitleBarLayer* titlebar = TitleBarLayer::node();
            CC_BREAK_IF(!titlebar);
            titlebar->setPosition(CCPointMake(0, 480.0f - 73.0f));
            addChild(titlebar,kZTitleBarLayer,kTagTitleBarLayer);
            
            PaintingReplayLayer* replayLayer = PaintingReplayLayer::node();
            CC_BREAK_IF(!replayLayer);
            replayLayer->setPosition(CCPointMake(0, 149.0f ));
            addChild(replayLayer,kZReplayLayer,kTagReplayLayer);
            
            SolveKeyboardLayer* keyboardLayer = SolveKeyboardLayer::node();
            CC_BREAK_IF(!keyboardLayer);
            keyboardLayer->setPosition(CCPointZero);
            addChild(keyboardLayer,kZKeyboardLayer,kTagKeyboardLayer);
            
            result = true;
        } while (0);
        return result;
    }
    
    void SolvingLayer::onEnter() {
        CCLayer::onEnter();
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPrioritySolvingLayer, true);
    }
    
    void SolvingLayer::onExit() {
        CCLayer::onExit();
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        
        // reset all 
        static_cast<CanvasLayer*>(getChildByTag(kTagReplayLayer))->resetToInitial();
    }
    
}





