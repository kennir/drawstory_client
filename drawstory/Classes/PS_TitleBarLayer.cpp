//
//  PS_TitleBarLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PS_TitleBarLayer.h"
#include "types.h"
#include "PaintingCanvasLayer.h"
#include "GlobalData.h"
#include "UserProfile.h"
#include "Game.h"

using namespace cocos2d;



namespace PaintingScene {
    
    enum {
        kZLabel = 1,
    };
    
    enum { 
        kTagTurnLabel = 100,
        kTagDescriptLabel,
        
        // hit test begin
        kTagColor1,
        kTagColor2,
        kTagColor3,
        kTagColor4,
        kTagMoreButton,
        // hit test end
        
        kTagColorMin = kTagColor1,
        kTagColorMax = kTagColor4,
        
        kTagButtonMin = kTagColor1,
        kTagButtonMax = kTagMoreButton,
    };
    
    TitleBarLayer::TitleBarLayer()
    : trackingNode_(NULL) {
    }
    
    bool TitleBarLayer::init() { 
        bool result = false;
        do {
            CC_BREAK_IF(!CCLayer::init());
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            
            CCSprite* title = CCSprite::spriteWithSpriteFrameName("pl_background_title");
            CC_BREAK_IF(!title);
            title->setPosition(CCPointMake(winSize.width * 0.5f, title->getContentSize().height * 0.5f));
            addChild(title);
            
            CCPoint turnPosition = CCPointMake(25.0f,75.0f);
            CCLabelTTF* turnLabel = CCLabelTTF::labelWithString("0", "Arial-BoldMT", 24.0f);
            turnLabel->setPosition(turnPosition);
            turnLabel->setColor(ccWHITE);
            addChild(turnLabel,kZLabel,kTagTurnLabel);
            
            Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
            CC_ASSERT(game != NULL);
            const Question& question = game->question();
            const Question::Word& word = question.word(GlobalData::sharedGlobalData()->currentDifficult());
            std::ostringstream oss;
            oss << "你正在为" << game->otherPlayerName() << "绘制" << word.word;
            
            CCLabelTTF* descLabel = CCLabelTTF::labelWithString(oss.str().c_str(), 
                                                                CCSizeMake(210.0f, 38.0f), 
                                                                CCTextAlignmentLeft, 
                                                                "Arial", 
                                                                12.0f);
            descLabel->setPosition(CCPointMake(205.0f, 75.0f));
            descLabel->setColor(ccBLACK);
            addChild(descLabel,kZLabel,kTagDescriptLabel);
            
            oss.str("");
            oss << "Tips:" << word.prompt;
            
            CCLabelTTF* promptLabel = CCLabelTTF::labelWithString(oss.str().c_str(), 
                                                                  CCSizeMake(210.0f, 38.0f), 
                                                                  CCTextAlignmentLeft, 
                                                                  "Arial", 
                                                                  12.0f);
            promptLabel->setPosition(CCPointMake(205.0f, 55.0f));
            promptLabel->setColor(ccBLACK);
            addChild(promptLabel,kZLabel,kTagDescriptLabel);
            
            CCPoint color1Pos = CCPointMake(25.0f, 20.0f);
            CCSprite* color1 = CCSprite::spriteWithSpriteFrameName("pl_c_black");
            color1->setPosition(color1Pos);
            addChild(color1,kZLabel,kTagColor1);
            
            CCPoint color2Pos = CCPointMake(60.0f, 20.0f);
            CCSprite* color2 = CCSprite::spriteWithSpriteFrameName("pl_c_red");
            color2->setPosition(color2Pos);
            addChild(color2,kZLabel,kTagColor2);
            
            CCPoint color3Pos = CCPointMake(95.0f, 20.0f);
            CCSprite* color3 = CCSprite::spriteWithSpriteFrameName("pl_c_yellow");
            color3->setPosition(color3Pos);
            addChild(color3,kZLabel,kTagColor3);
            
            CCPoint color4Pos = CCPointMake(130.0f, 20.0f);
            CCSprite* color4 = CCSprite::spriteWithSpriteFrameName("pl_c_blue");
            color4->setPosition(color4Pos);
            addChild(color4,kZLabel,kTagColor4);
            
            CCPoint morePos = CCPointMake(200, 20.0f);
            CCSprite* more = CCSprite::spriteWithSpriteFrameName("pl_morecolor");
            more->setPosition(morePos);
            addChild(more,kZLabel,kTagMoreButton);
            
            
            result = true;
        } while (0);
        return result;
    }
    
    void TitleBarLayer::onEnter() {
        CCLayer::onEnter();
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityTitleBarLayer, true);
    }
    
    void TitleBarLayer::onExit() {
        CCLayer::onExit();
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    }
    
    bool TitleBarLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        bool handled = false;
        
        trackingNode_ = hitTest(convertTouchToNodeSpace(touch));
        if(trackingNode_ != NULL){
            trackingNode_->setScale(0.9f);
            handled = true;
        }
        return handled;
    }
    
    void TitleBarLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(trackingNode_) {
            CCNode* node = hitTest(convertTouchToNodeSpace(touch));
            if(node != trackingNode_) {
                trackingNode_->setScale(1.0f);
                trackingNode_ = NULL;
            }
        }
    }
    
    void TitleBarLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(trackingNode_) {
            CCNode* node = hitTest(convertTouchToNodeSpace(touch));
            if(node == trackingNode_) {
                onNodeClicked(node);
            }
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        }
    }
    
    void TitleBarLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(trackingNode_) {
            CCNode* node = hitTest(convertTouchToNodeSpace(touch));
            if(node != trackingNode_) {
                trackingNode_->setScale(1.0f);
                trackingNode_ = NULL;
            }
        }
    }
    
    CCNode* TitleBarLayer::hitTest(const cocos2d::CCPoint &localPos){
        CCNode* node = NULL;
        for(int tag = kTagButtonMin; tag <= kTagButtonMax; ++tag) {
            CCNode* testing = getChildByTag(tag);
            if(CCRect::CCRectContainsPoint(testing->boundingBox(), localPos)) {
                node = testing;
                break;
            }
        }
        return node;
    }
    
    
    void TitleBarLayer::onNodeClicked(cocos2d::CCNode *button) {
        int tag = button->getTag();
        switch(tag) {
            case kTagColor1:
                canvas_->setPaintMode(kPaintModeDraw);
                canvas_->brush()->setColor(ccBLACK);
                break;
            case kTagColor2:
                canvas_->setPaintMode(kPaintModeDraw);
                canvas_->brush()->setColor(ccRED);
                break;
            case kTagColor3:
                canvas_->setPaintMode(kPaintModeDraw);
                canvas_->brush()->setColor(ccYELLOW);
                break;
            case kTagColor4:
                canvas_->setPaintMode(kPaintModeDraw);
                canvas_->brush()->setColor(ccBLUE);
                break;
            default:
                break;
        }
    }
    
    
}
