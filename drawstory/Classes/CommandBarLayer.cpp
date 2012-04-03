//
//  CommandBarLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CommandBarLayer.h"
#include "types.h"
#include "ToggleButton.h"
#include "Brush.h"
#include "CanvasLayer.h"

using namespace cocos2d;

// duration for expand buttons
static const float kExpandStep = 40.0f;
static const float kExpandDuration = 0.2f;

// Z Value
enum { 
    kZCommandBarButton = 3, 
};


// Tags
enum {     
    kTagButtonPencil        = 0,
    kTagButtonBrushWidth1,
    kTagButtonBrushWidth2,
    kTagButtonBrushWidth3,
    kTagButtonBrushWidth4,
    
    kTagButtonEraser,
    kTagButtonEraserWidth1,
    kTagButtonEraserWidth2,
    kTagButtonEraserWidth3,
    kTagButtonEraserWidth4,
    
    kTagButtonDelete,
    kTagButtonDone,
    kTagButtonSetup,
    
    kTagButtonMin = kTagButtonPencil,
    kTagButtonMax = kTagButtonSetup,
    
    kTagButtonBrushWidthMin = kTagButtonBrushWidth1,
    kTagButtonBrushWidthMax = kTagButtonBrushWidth4,
    kTagButtonBrushWidthCount = (kTagButtonBrushWidthMax - kTagButtonBrushWidthMin + 1),
    
    kTagButtonEraserWidthMin = kTagButtonEraserWidth1,
    kTagButtonEraserWidthMax = kTagButtonEraserWidth4,
    kTagButtonEraserWidthCount = (kTagButtonEraserWidthMax - kTagButtonEraserWidthMin + 1),
};

CommandBarLayer::CommandBarLayer() 
: trackingNode_(NULL)
, brushWidthMenuExpanded_(false)
, eraserWidthMenuExpanded_(false) 
, canvas_(NULL)
{
    
}


bool CommandBarLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite* cmdbar = CCSprite::spriteWithSpriteFrameName("pl_black_mask");
        CC_BREAK_IF(!cmdbar);
        cmdbar->setPosition(CCPointMake(winSize.width * 0.5f, cmdbar->getContentSize().height * 0.5f));
        addChild(cmdbar);
        
        
        CCPoint pencilPosition = CCPointMake(28.0f,28.0f);
        
        
        // create each color buttons
        CCPoint brushWidthPosition = pencilPosition;
        ToggleButton* brushWidth1 = ToggleButton::toggleButtonWithImage("pl_ball_1x_white", "pl_ball_1x_blue");
        CC_BREAK_IF(!brushWidth1);
        brushWidth1->setPosition(brushWidthPosition);
        addChild(brushWidth1,kZCommandBarButton,kTagButtonBrushWidth1);
        brushWidth1->setIsVisible(false);
        
        ToggleButton* brushWidth2 = ToggleButton::toggleButtonWithImage("pl_ball_2x_white", "pl_ball_2x_blue");
        CC_BREAK_IF(!brushWidth2);
        brushWidth2->setPosition(brushWidthPosition);
        addChild(brushWidth2,kZCommandBarButton,kTagButtonBrushWidth2);
        brushWidth2->setIsVisible(false);
        
        ToggleButton* brushWidth3 = ToggleButton::toggleButtonWithImage("pl_ball_3x_white", "pl_ball_3x_blue");
        CC_BREAK_IF(!brushWidth3);
        brushWidth3->setPosition(brushWidthPosition);
        addChild(brushWidth3,kZCommandBarButton,kTagButtonBrushWidth3);
        brushWidth3->setIsVisible(false);
        
        ToggleButton* brushWidth4 = ToggleButton::toggleButtonWithImage("pl_ball_4x_white", "pl_ball_4x_blue");
        CC_BREAK_IF(!brushWidth4);
        brushWidth4->setPosition(brushWidthPosition);
        addChild(brushWidth4,kZCommandBarButton,kTagButtonBrushWidth4);
        brushWidth4->setIsVisible(false);
        
        ToggleButton* pencil = ToggleButton::toggleButtonWithImage("pl_pencil_c", "pl_pencil_s");
        CC_BREAK_IF(!pencil);
        pencil->setPosition(pencilPosition);
        addChild(pencil,kZCommandBarButton,kTagButtonPencil);
        
        
        
        CCPoint eraserPosition = CCPointMake(78.0f, 28.0f);
        
        // create each color buttons
        CCPoint eraserWidthPosition = eraserPosition;
        ToggleButton* eraserWidth1 = ToggleButton::toggleButtonWithImage("pl_ball_1x_white", "pl_ball_1x_blue");
        CC_BREAK_IF(!eraserWidth1);
        eraserWidth1->setPosition(eraserWidthPosition);
        addChild(eraserWidth1,kZCommandBarButton,kTagButtonEraserWidth1);
        eraserWidth1->setIsVisible(false);
        
        ToggleButton* eraserWidth2 = ToggleButton::toggleButtonWithImage("pl_ball_2x_white", "pl_ball_2x_blue");
        CC_BREAK_IF(!eraserWidth2);
        eraserWidth2->setPosition(eraserWidthPosition);
        addChild(eraserWidth2,kZCommandBarButton,kTagButtonEraserWidth2);
        eraserWidth2->setIsVisible(false);
        
        ToggleButton* eraserWidth3 = ToggleButton::toggleButtonWithImage("pl_ball_3x_white", "pl_ball_3x_blue");
        CC_BREAK_IF(!eraserWidth3);
        eraserWidth3->setPosition(eraserWidthPosition);
        addChild(eraserWidth3,kZCommandBarButton,kTagButtonEraserWidth3);
        eraserWidth3->setIsVisible(false);
        
        ToggleButton* eraserWidth4 = ToggleButton::toggleButtonWithImage("pl_ball_4x_white", "pl_ball_4x_blue");
        CC_BREAK_IF(!eraserWidth4);
        eraserWidth4->setPosition(eraserWidthPosition);
        addChild(eraserWidth4,kZCommandBarButton,kTagButtonEraserWidth4);
        eraserWidth4->setIsVisible(false);
        
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

void CommandBarLayer::onEnter() {
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityCommandBarLayer, true);
}

void CommandBarLayer::onExit() {
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}


CCNode* CommandBarLayer::hitTest(const cocos2d::CCPoint &localPos) {
    CCNode* touched = NULL;
    for(int begin = kTagButtonMin; begin <= kTagButtonMax; ++begin){
        CCSprite* button = static_cast<CCSprite*>(getChildByTag(begin));
        CC_ASSERT(button != NULL);
        
        if(CCRect::CCRectContainsPoint(button->boundingBox(), localPos) && button->getIsVisible()){
            touched = static_cast<CCNode*>(button);
            break;
        }
    }
    return touched;
}

void CommandBarLayer::beginTouchNode(cocos2d::CCNode *button) {
    int tag = button->getTag();
    switch (tag) {
        case kTagButtonPencil:
            (static_cast<ToggleButton*>(button))->setToggled(true);
            break;
            
        default:
            button->setScale(0.9f);
            break;
    }
}

void CommandBarLayer::endTouchNode(cocos2d::CCNode *button) {
    int tag = button->getTag();
    switch (tag) {
        case kTagButtonPencil:
            (static_cast<ToggleButton*>(button))->setToggled(false);
            break;
            
        default:
            button->setScale(1.0f);
            break;
    }
}

void CommandBarLayer::onNodeClicked(cocos2d::CCNode *button) {
    int tag = button->getTag();
    switch (tag) {
        case kTagButtonPencil:
            if(!brushWidthMenuExpanded_) {
                prepareWidthMenuItems(canvas_->brush(), kTagButtonBrushWidthMin, kTagButtonBrushWidthMax);
                slideUpButtons(kTagButtonBrushWidthMin,kTagButtonBrushWidthMax,button->getPosition());
                brushWidthMenuExpanded_ = true;
            } else
                collapseBrushWidthMenu();
            
            static_cast<ToggleButton*>(button)->setToggled(brushWidthMenuExpanded_);
            
            collapseEraserWidthMenu();
            break;
        case kTagButtonEraser:
            if(!eraserWidthMenuExpanded_){
                prepareWidthMenuItems(canvas_->eraser(), kTagButtonEraserWidthMin, kTagButtonEraserWidthMax);
                slideUpButtons(kTagButtonEraserWidthMin, kTagButtonEraserWidthMax, button->getPosition());
                eraserWidthMenuExpanded_ = true;
            } else
                collapseEraserWidthMenu();

            collapseBrushWidthMenu();
            break;
        case kTagButtonDelete:
            collapseEraserWidthMenu();
            collapseBrushWidthMenu();
            // reset canvas
            canvas_->reset();
            break;
        case kTagButtonDone:
            collapseEraserWidthMenu();
            collapseBrushWidthMenu();
            // done, send the 
            
            break;
            
        default:
            break;
    }
    
}

void CommandBarLayer::slideUpButtons(int beginTag,int endTag,const CCPoint& moveFrom) {
    CCPoint moveToPos = moveFrom;
    for(int btnTag = beginTag; btnTag <= endTag; ++btnTag) {   
        moveToPos.y += kExpandStep;
        cocos2d::CCNode* button = getChildByTag(btnTag);
        
        button->stopAllActions();
        button->setIsVisible(true);
        button->runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(kExpandDuration, moveToPos),kExpandDuration / 2));
    }
}


void CommandBarLayer::slideDownButtons(int beginTag, int endTag, const cocos2d::CCPoint &moveTo) {
    for(int btnTag = beginTag; btnTag <= endTag; ++btnTag) {
        CCNode* button = getChildByTag(btnTag);
        
        button->stopAllActions();
        button->runAction(CCSequence::actionOneTwo(
                                                   CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(kExpandDuration, moveTo),kExpandDuration / 2),
                                                   CCHide::action()));
    }
}

bool CommandBarLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    bool handled = false;
    
    trackingNode_ = hitTest(convertTouchToNodeSpace(touch));
    if(trackingNode_ != NULL){
        beginTouchNode(trackingNode_);
        handled = true;
    } else {
        collapseBrushWidthMenu();
        collapseEraserWidthMenu();
    }
    return handled;
}

void CommandBarLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        CCNode* node = hitTest(convertTouchToNodeSpace(touch));
        if(node != trackingNode_){
            endTouchNode(trackingNode_);
            trackingNode_ = NULL;
        }
    }
}

void CommandBarLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        endTouchNode(trackingNode_); // call endTouchNode before onNodeClicked
        
        CCNode* node = hitTest(convertTouchToNodeSpace(touch));
        if(node == trackingNode_){
            onNodeClicked(node);
        }
        
        trackingNode_ = NULL;
    }
}

void CommandBarLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_){
        endTouchNode(trackingNode_);
        trackingNode_ = NULL;
    }
}


void CommandBarLayer::prepareWidthMenuItems(Brush* brush,int beginTag,int endTag) {
    for(int tag = beginTag; tag <= endTag; ++tag) {
        static_cast<ToggleButton*>(getChildByTag(tag))->setToggled(false);
    }
    
    switch (brush->width()) {
        case kBrushWidth1:
            static_cast<ToggleButton*>(getChildByTag(beginTag + 0))->setToggled(true);
            break;
        case kBrushWidth2:
            static_cast<ToggleButton*>(getChildByTag(beginTag + 1))->setToggled(true);
            break;
        case kBrushWidth3:
            static_cast<ToggleButton*>(getChildByTag(beginTag + 2))->setToggled(true);
            break;
        case kBrushWidth4:
            static_cast<ToggleButton*>(getChildByTag(beginTag + 3))->setToggled(true);
            break;
        default:
            break;
    }
}


void CommandBarLayer::collapseBrushWidthMenu() {
    if(brushWidthMenuExpanded_) {
        slideDownButtons(kTagButtonBrushWidthMin, 
                         kTagButtonBrushWidthMax, 
                         getChildByTag(kTagButtonPencil)->getPosition());
        brushWidthMenuExpanded_ = false;
        
        static_cast<ToggleButton*>(getChildByTag(kTagButtonPencil))->setToggled(false);
    }
}

void CommandBarLayer::collapseEraserWidthMenu() {
    if(eraserWidthMenuExpanded_) {
        slideDownButtons(kTagButtonEraserWidthMin, 
                         kTagButtonEraserWidthMax, 
                         getChildByTag(kTagButtonEraser)->getPosition());
        eraserWidthMenuExpanded_ = false;
    }
}
