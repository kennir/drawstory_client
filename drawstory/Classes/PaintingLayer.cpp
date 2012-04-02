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


enum { kZCanvasLayer = 1,kZCommandBar = 2,kZCommandBarButton = 3, };



enum { 
    kTagCanvasLayer = 0,
    
    kTagButtonPencil,
    kTagButtonEraser,
    kTagButtonDelete,
    kTagButtonDone,
    kTagButtonSetup,
    
    kTagButtonMin = kTagButtonPencil,
    kTagButtonMax = kTagButtonSetup
};

PaintingLayer::PaintingLayer() : canvas_(NULL),trackingNode_(NULL) { 
}

PaintingLayer::~PaintingLayer() {
    CC_SAFE_RELEASE(canvas_);
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
        canvas_ = CanvasLayer::node();
        CC_BREAK_IF(!canvas_);
        canvas_->retain();
        canvas_->setPosition(canvasPosition);
        addChild(canvas_,kZCanvasLayer,kTagCanvasLayer);
        
        CCSprite* cmdbar = CCSprite::spriteWithSpriteFrameName("pl_black_mask");
        CC_BREAK_IF(!cmdbar);
        cmdbar->setPosition(CCPointMake(winSize.width * 0.5f, cmdbar->getContentSize().height * 0.5f));
        addChild(cmdbar,kZCommandBar);
        
        
        CCPoint pencilPosition = CCPointMake(28.0f,28.0f);
        CCSprite* pencil = CCSprite::spriteWithSpriteFrameName("pl_pencil_s");
        CC_BREAK_IF(!pencil);
        pencil->setPosition(pencilPosition);
        addChild(pencil,kZCommandBarButton,kTagButtonPencil);

        CCPoint eraserPosition = CCPointMake(78.0f, 28.0f);
        CCSprite* eraser = CCSprite::spriteWithSpriteFrameName("pl_eraser");
        CC_BREAK_IF(!eraser);
        eraser->setPosition(eraserPosition);
        addChild(eraser,kZCommandBarButton,kTagButtonEraser);
        
        CCPoint deletePosition = CCPointMake(128.0f, 28.0f);
        CCSprite* del = CCSprite::spriteWithSpriteFrameName("pl_del");
        CC_BREAK_IF(!del);
        del->setPosition(deletePosition);
        addChild(del,kZCommandBarButton,kTagButtonDelete);
        
        CCPoint donePosition = CCPointMake(220.0f, 28.0f);
        CCSprite* done = CCSprite::spriteWithSpriteFrameName("pl_down");
        CC_BREAK_IF(!done);
        done->setPosition(donePosition);
        addChild(done,kZCommandBarButton,kTagButtonDone);
        
        CCPoint setupPosition = CCPointMake(300.0f, 28.0f);
        CCSprite* setup = CCSprite::spriteWithSpriteFrameName("pl_setup");
        CC_BREAK_IF(!setup);
        setup->setPosition(setupPosition);
        addChild(setup,kZCommandBarButton,kTagButtonSetup);
        
        
    
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
    trackingNode_ = hitTest(convertTouchToNodeSpace(touch));
    if(trackingNode_ != NULL){
        trackingNode_->setScale(0.9f);
    }
    return true;
}

void PaintingLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        CCNode* node = hitTest(convertTouchToNodeSpace(touch));
        if(node != trackingNode_){
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        }
    }
}

void PaintingLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        CCNode* node = hitTest(convertTouchToNodeSpace(touch));
        if(node == trackingNode_){
            
        }
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

void PaintingLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

CCNode* PaintingLayer::hitTest(const cocos2d::CCPoint &localPos) {
    CCNode* touched = NULL;
    for(int begin = kTagButtonMin; begin <= kTagButtonMax; ++begin){
        CCSprite* button = static_cast<CCSprite*>(getChildByTag(begin));
        CC_ASSERT(button != NULL);
        
        if(CCRect::CCRectContainsPoint(button->boundingBox(), localPos)){
            touched = static_cast<CCNode*>(button);
            break;
        }
    }
    return touched;
}

