//
//  PaintingReplayLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintingReplayLayer.h"


PaintingReplayLayer::PaintingReplayLayer()
: timePassed_(0.0f) {
    
}
    
bool PaintingReplayLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CanvasLayer::init());
        
        if(!commandQueue().commands().empty())
        {
            currentIt_ = commandQueue().commands().begin();
            scheduleUpdate();
        }
        
        result = true;
    } while (0);
    return result;
}


void PaintingReplayLayer::update(cocos2d::ccTime dt) {
    timePassed_ += dt;
    // check commands
    if(currentIt_ != commandQueue().commands().end()) {

        
    }
}



