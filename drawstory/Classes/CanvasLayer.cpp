//
//  CanvasLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CanvasLayer.h"

using namespace cocos2d;




enum { kTagRenderTexture = 1000 };
enum { kCanvasHeight = 333 };

CanvasLayer::CanvasLayer() 
: target_(NULL) {    
}


CanvasLayer::~CanvasLayer() {
    CC_SAFE_RELEASE(target_);
}


bool CanvasLayer::init(){
    bool result = false; 
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        target_ = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, kCanvasHeight);
        CC_BREAK_IF(!target_);
        target_->retain();
        target_->setPosition(CCPointMake(winSize.width * 0.5f, kCanvasHeight * 0.5f));
        addChild(target_,0,kTagRenderTexture);
        
        
        // initialize texture to white
        target_->beginWithClear(255, 255, 255, 255);
        target_->end(true);

        
        setContentSize(CCSizeMake(winSize.width,kCanvasHeight));
        layerRect_ = CCRectMake(0, 0, winSize.width, kCanvasHeight);
        
        result = true;
    } while (0);
    return result;
}
    
void CanvasLayer::resetToInitial() {
    commandQueue_.clear();
    
    target_->beginWithClear(255, 255, 255, 255);
    target_->end(true);
}

void CanvasLayer::deletePainting() {
    commandQueue_.push(new ResetCommand);
    
    target_->beginWithClear(255, 255, 255, 255);
    target_->end(true);
}

    

