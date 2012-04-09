//
//  PaintingReplayLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintingReplayLayer.h"
#include "Game.h"
#include "UserProfile.h"
#include "GlobalData.h"


PaintingReplayLayer::PaintingReplayLayer()
: timePassed_(0.0f) {
    
}
    
bool PaintingReplayLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CanvasLayer::init());
        
        
        
        result = true;
    } while (0);
    return result;
}


void PaintingReplayLayer::onEnter() {
    CanvasLayer::onEnter();
    
    // load replay
    Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
    CC_ASSERT(game != NULL);
    const Replay& replay = game->replay();
    commandQueue_.deserialize(replay.paintReplay().d(), replay.paintReplay().originSize);
    

}


void PaintingReplayLayer::update(cocos2d::ccTime dt) {
    timePassed_ += dt;

}



