//
//  PaintingScene_PaintingCanvasLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintingScene_PaintingCanvasLayer.h"

using namespace cocos2d;

namespace PaintingScene {
    
    
    PaintingCanvasLayer::PaintingCanvasLayer()
    : brush_(NULL)
    , eraser_(NULL)
    , paintMode_(kPaintModeDraw)
    , drawing_(false){
    }
    
    PaintingCanvasLayer::~PaintingCanvasLayer() {
        delete brush_;
        delete eraser_;
    }
    
    
    bool PaintingCanvasLayer::init() { 
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
    
    void PaintingCanvasLayer::resetToInitial() {
        CanvasLayer::resetToInitial();
        
        brush_->setWidth(kBrushWidth2);
        brush_->setColor(ccBLACK);
        eraser_->setWidth(kBrushWidth2);
    }
    
    
    void PaintingCanvasLayer::onEnter() {
        CanvasLayer::onEnter();
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityCanvasLayer, true);
    }
    
    void PaintingCanvasLayer::onExit() {
        CanvasLayer::onExit();
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    }
    
    
    bool PaintingCanvasLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        CCPoint localPos = convertTouchToNodeSpace(touch);
        if (CCRect::CCRectContainsPoint(layerRect_, localPos)) {
            drawing_ = true;
            previousLocalPosition_ = localPos;
            
            if(paintMode_ == kPaintModeDraw)
                commandQueue_.beginCommand(new DrawCommand(brush_->width(),brush_->color()));
            else 
                commandQueue_.beginCommand(new EraseCommand);
            return true;
        }
        
        return false;
    }
    
    void PaintingCanvasLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(drawing_){
            CCPoint localPos = convertTouchToNodeSpace(touch);
            //        if(CCRect::CCRectContainsPoint(layerRect_, localPos)) {
            
            float distance = ccpDistance(previousLocalPosition_, localPos);
            if(distance > 1){
                // push to command
                static_cast<PointsCommand*>(commandQueue_.current())->push(localPos);
                
                Brush* brush = (paintMode_ == kPaintModeDraw) ? brush_ : eraser_;
                
                target_->begin();
                
                int d = static_cast<int>(distance);
                for(int i = 0; i < d; ++i){
                    float difx = localPos.x - previousLocalPosition_.x;
                    float dify = localPos.y - previousLocalPosition_.y;
                    float delta = static_cast<float>(i) / distance;
                    brush->setPosition(CCPointMake(previousLocalPosition_.x + (difx * delta),
                                                   previousLocalPosition_.y + (dify * delta)));
                    brush->visit();
                }
                target_->end(false);
                previousLocalPosition_ = localPos;
            }
            //        } else {
            //            drawing_ = false;
            //            
            //            target_->begin();
            //            target_->end(true);
            //        }
        }
    }
    
    
    
    void PaintingCanvasLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(drawing_){
            CCPoint localPos = convertTouchToNodeSpace(touch);
            if(CCRect::CCRectContainsPoint(layerRect_, localPos)) {
                
                static_cast<PointsCommand*>(commandQueue_.current())->push(localPos);
                
                Brush* brush = (paintMode_ == kPaintModeDraw) ? brush_ : eraser_;
                
                target_->begin();
                brush->setPosition(localPos);
                brush->visit();
                target_->end(true);
            }
            
            drawing_ = false;
        }
    }
    
    void PaintingCanvasLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(drawing_){
            drawing_ = false;
            
            target_->begin();
            target_->end(true);
        }
    }

    
    
}

