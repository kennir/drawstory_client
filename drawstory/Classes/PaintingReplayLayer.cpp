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


using namespace cocos2d;


PaintingReplayLayer::PaintingReplayLayer()
: timePassed_(0.0f)
, paintMode_(kPaintModeDraw)
, brush_(NULL)
, eraser_(NULL)
{
}

PaintingReplayLayer::~PaintingReplayLayer(){
    delete brush_;
    delete eraser_;
}
    
bool PaintingReplayLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CanvasLayer::init());
        
        brush_ = new Brush;
        CC_BREAK_IF(!brush_ || !brush_->init());
        
        eraser_ = new Brush;
        CC_BREAK_IF(!eraser_ || !eraser_->init());
        eraser_->setColor(ccWHITE); // eraser is white always
        

        
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
    
    if(!commandQueue().empty()) {
        currentPos_ = commandQueue().commands().begin();
        scheduleUpdate();
    }
}


void PaintingReplayLayer::update(cocos2d::ccTime dt) {
    timePassed_ += static_cast<long>(dt * 1000);
    while(timePassed_ > (*currentPos_)->millisecond()) {
        processCommand(*currentPos_);
        ++currentPos_;
        
        if(currentPos_ == commandQueue().commands().end()){
            unscheduleUpdate();
            break;
        }
    }
}

void PaintingReplayLayer::processCommand(const Command *command) {
    switch (command->type()) {
        case kCommandTypeReset:
            deletePainting();
            break;
        case kCommandTypeSetPaintMode:
            paintMode_ = static_cast<const SetPaintModeCommand*>(command)->paintMode();
            break;
        case kCommandTypeSetColor:
            brush_->setColor(static_cast<const SetColorCommand*>(command)->color());
            break;
        case kCommandTypeSetWidth:
            if(paintMode_ == kPaintModeDraw) 
                brush_->setWidth(static_cast<const SetWidthCommand*>(command)->width());
            else 
                eraser_->setWidth(static_cast<const SetWidthCommand*>(command)->width());
            break;
        case kCommandTypeTouchBegan:
            previousPosition_ = static_cast<const TouchCommand*>(command)->touchedPoint();
            break;
        case kCommandTypeTouchMoved:
            processCommandTouchMoved(static_cast<const TouchCommand*>(command));
            break;
        case kCommandTypeTouchEnded:
            processCommandTouchEnded(static_cast<const TouchCommand*>(command));
            break;
        default:
            break;
    }
}

void PaintingReplayLayer::processCommandTouchMoved(const TouchCommand *command) {
    float distance = ccpDistance(previousPosition_, command->touchedPoint());
    if(distance > 1){        
        Brush* brush = (paintMode_ == kPaintModeDraw) ? brush_ : eraser_;
        
        target_->begin();
        
        int d = static_cast<int>(distance);
        for(int i = 0; i < d; ++i){
            float difx = command->touchedPoint().x - previousPosition_.x;
            float dify = command->touchedPoint().y - previousPosition_.y;
            float delta = static_cast<float>(i) / distance;
            brush->setPosition(CCPointMake(previousPosition_.x + (difx * delta),
                                           previousPosition_.y + (dify * delta)));
            brush->visit();
        }
        target_->end(false);
        previousPosition_ = command->touchedPoint();
    }
}

void PaintingReplayLayer::processCommandTouchEnded(const TouchCommand *command) {
//    Brush* brush = (paintMode_ == kPaintModeDraw) ? brush_ : eraser_;
    
    target_->begin();
//    brush->setPosition(command->touchedPoint());
//    brush->visit();
    target_->end(true);
}



