//
//  GameListNode.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameListLayer.h"
#include "GameLabelNode.h"
#include "UserProfile.h"
#include "Game.h"
#include "types.h"
#include "LobbyScene.h"
#include "LobbySceneLogic.h"

using namespace cocos2d;

enum { kZButton = 1, kZGameLabel = 1 };
enum { kTagStartGameButton = 100,kTagGameLabel };

// layer heigth of empty list
static const float kLayerHeightOfEmptyList = 132.0f;
static const float kGameLabelHeight = 67.0f;


// ribber height
static const float kElasticLength = 40.0f;
static const float kAutoMoveLength = 5.0f;

GameListLayer::GameListLayer() 
: touchTarget_(kTouchTargetNone)
, trackingNode_(NULL) {
}

GameListLayer::~GameListLayer() {
    removeAllNodes();
    while(!pool_.empty()){
        pool_.top()->release();
        pool_.pop();
    }
}

bool GameListLayer::init(){
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //== start button
        CCPoint startPos = CCPointMake(0, -28.0f);
        CCSprite* startgame = CCSprite::spriteWithSpriteFrameName("startgame");
        CC_BREAK_IF(!startgame);
        startgame->setPosition(startPos);
        addChild(startgame,kZButton,kTagStartGameButton);
        
        //== playing game
        CCPoint gameplayPos = CCPointMake(0, -104.0f);
        CCSprite* gameplay = CCSprite::spriteWithSpriteFrameName("gameplay");
        CC_BREAK_IF(!gameplay);
        gameplay->setPosition(gameplayPos);
        addChild(gameplay);

        setContentSize(CCSizeMake(winSize.width, kLayerHeightOfEmptyList));
        
        result = true;
    } while (0);
    return result;
}

void GameListLayer::setOriginPosition(const cocos2d::CCPoint &newOriginPos) {
    originPosition_ = newOriginPos;
    setPosition(newOriginPos);
}

void GameListLayer::onEnter() {
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityGameListLayer, true);
}

void GameListLayer::onExit() {
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool GameListLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {   
    CCPoint localPos = convertTouchToNodeSpace(touch);
    stopAllActions();
    trackingNode_ = hitTestWithGameLabel(localPos);
    if(trackingNode_ 
       && static_cast<GameLabelNode*>(trackingNode_)->onTouchBegan(trackingNode_->convertTouchToNodeSpace(touch))){
        touchTarget_ = kTouchTargetGameLabel;
    } else if((trackingNode_ = hitTest(localPos)) != NULL){
        trackingNode_->setScale(0.9f);
        touchTarget_ = kTouchTargetButton;
    } else {
        beginLocalPos_ = localPos;
        previousPosY_ = getPositionY();
        touchTarget_ = kTouchTargetLayer;
    }
    

    return true;
}

void GameListLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(touchTarget_ != kTouchTargetNone) {
        
        if(touchTarget_ == kTouchTargetGameLabel) {
            CC_ASSERT(trackingNode_ != NULL);
            static_cast<GameLabelNode*>(trackingNode_)->onTouchMoved(trackingNode_->convertTouchToNodeSpace(touch));
        } else if(touchTarget_ == kTouchTargetButton) {
            CCPoint localPos = convertTouchToNodeSpace(touch);
            if(hitTest(localPos) != trackingNode_) {
                trackingNode_->setScale(1.0f);
                trackingNode_ = NULL;
                touchTarget_ = kTouchTargetNone;
            }
        } else if(touchTarget_ == kTouchTargetLayer)  {
            CCPoint localPos = convertTouchToNodeSpace(touch);
            float distance = localPos.y - beginLocalPos_.y;
            float height = getContentSize().height;
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            height = max(winSize.height - kLayerHeightOfEmptyList,height);
            
            float y = getPositionY();
            y += distance;
            if(y - height > kElasticLength)
                y = height + kElasticLength;
            else if(y < originPosition_.y - kElasticLength)
                y = originPosition_.y - kElasticLength;
            
            setPositionY(y);
        }
    }
}

void GameListLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(touchTarget_ != kTouchTargetNone) {
        if(touchTarget_ == kTouchTargetGameLabel) {
            CC_ASSERT(trackingNode_ != NULL);
            static_cast<GameLabelNode*>(trackingNode_)->onTouchEnded(trackingNode_->convertTouchToNodeSpace(touch));
            trackingNode_ = NULL;
        } else if(touchTarget_ == kTouchTargetButton) {
            CCPoint localPos = convertTouchToNodeSpace(touch);
            if(hitTest(localPos) == trackingNode_) {
                onNodeClicked(trackingNode_);
            }
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        } else if(touchTarget_ == kTouchTargetLayer) {
            CCPoint localPos = convertTouchToNodeSpace(touch);
            float distance = localPos.y - beginLocalPos_.y;
            float height = getContentSize().height;
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            height = max(winSize.height - kLayerHeightOfEmptyList,height);
            
            float y = getPositionY();
            y += distance;
            if(y - height > 0)
                y = height;
            else if(y < originPosition_.y)
                y = originPosition_.y;
            else {
                float movedDistance = y - previousPosY_;
                
                if(movedDistance > kAutoMoveLength) {
                    //　move up
                    runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(1.0f, CCPointMake(originPosition_.x, height)), 0.5f));
                } else if(movedDistance < -kAutoMoveLength) {
                    runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(1.0f, CCPointMake(originPosition_.x, originPosition_.y)), 0.5f));
                }
            }
            setPositionY(y);
        }
        
        touchTarget_ = kTouchTargetNone;
    }
}



void GameListLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(touchTarget_ != kTouchTargetNone) {
        if(touchTarget_ == kTouchTargetButton) {
            CC_ASSERT(trackingNode_);
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        } else {
            float y = getPositionY();
            float height = getContentSize().height;
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            height = max(winSize.height - kLayerHeightOfEmptyList,height);
            
            if(y - height > 0)
                y = height;
            else if(y < originPosition_.y)
                y = originPosition_.y;
        }
        touchTarget_ = kTouchTargetNone;
    }
}

void GameListLayer::synchronizeGameList() {
    removeAllNodes();
    
    CCPoint position = CCPointMake(0,-kLayerHeightOfEmptyList - (kGameLabelHeight * 0.5f));
        
    UserProfile* user = UserProfile::sharedUserProfile();
    const std::list<Game*>& games = user->games();
    std::list<Game*>::const_iterator it = games.begin();
    for(; it != games.end(); ++it){
        GameLabelNode* label = createLabelNode();
        label->setPosition(position);
        label->setGame(*it);
        addChild(label,kZGameLabel,kTagGameLabel);
        gameLabels_.push_back(label);
        position.y -= label->getContentSize().height;
    }

    
    
    setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width,
                              (kGameLabelHeight * games.size()) + kLayerHeightOfEmptyList ));
}


void GameListLayer::synchronizeGameListForState() {
    
    UserProfile* user = UserProfile::sharedUserProfile();
    
    std::list<GameLabelNode*>::iterator it = gameLabels_.begin();
    for(; it != gameLabels_.end(); ++it) {
        Game* game = user->findGame((*it)->gameId());
        if(game) {
            (*it)->setGame(game);
        }
    }
}

void GameListLayer::removeAllNodes() {
    std::list<GameLabelNode*>::iterator it = gameLabels_.begin();
    while(it != gameLabels_.end()) {
        pool_.push(*it);
        removeChild(*it, false);
        it = gameLabels_.erase(it);
    }
}

GameLabelNode* GameListLayer::createLabelNode() {
    GameLabelNode* node = NULL;
    if(!pool_.empty()){
        node = pool_.top();
        pool_.pop();
    } else {
        node = static_cast<GameLabelNode*>(GameLabelNode::node());
        node->retain();
        node->setAnchorPoint(CCPointMake(0.5f, 0.5f));
    }
    return node;
}

CCNode* GameListLayer::hitTest(const cocos2d::CCPoint &localPos) {
    CCNode* node = NULL;
    // Check with start button
    CCNode* startGameButton = getChildByTag(kTagStartGameButton);
    if(CCRect::CCRectContainsPoint(startGameButton->boundingBox(), localPos)) {
        node = startGameButton;
    } 
    return node;
}

CCNode* GameListLayer::hitTestWithGameLabel(const cocos2d::CCPoint &localPos) {
    CCNode* node = NULL;
    std::list<GameLabelNode*>::iterator it = gameLabels_.begin();
    for(; it != gameLabels_.end(); ++it) {
        if(CCRect::CCRectContainsPoint((*it)->boundingBox(), localPos)) {
            node = *it;
            break;
        }
    }
    return node;
}


void GameListLayer::onNodeClicked(cocos2d::CCNode *button) {
    int tag = button->getTag();
    switch(tag) {
        case kTagStartGameButton:
            static_cast<LobbyScene*>(getParent())->logic()->createRandomGame();
            break;
    }
}


