//
//  GameLabelNode.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameLabelNode.h"
#include "Game.h"
#include <iostream>
#include <iomanip>
#include "types.h"
#include "UserProfile.h"
#include "LobbyScene.h"
#include "LobbySceneLogic.h"

using namespace cocos2d;

enum { kTagTurnLabel = 1000,kTagStateLabel,kTagNameLabel,kTagPencilButton };

CCNode* GameLabelNode::node() {
    GameLabelNode* newNode = new GameLabelNode;
    if(newNode && newNode->init()){
        newNode->autorelease();
    } else {
        delete newNode;
        newNode = NULL;
    }
    return newNode;
}


bool GameLabelNode::init()
{
    bool result = false;
    do {
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("ui_banner");
        CC_BREAK_IF(!background);
        CCSize backgroundSize = background->getContentSize();
        background->setPosition(CCPointMake(backgroundSize.width * 0.5f, backgroundSize.height * 0.5f));
        addChild(background);
        
        setContentSize(backgroundSize);
        
        CCSprite* pencil = CCSprite::spriteWithSpriteFrameName("ui_pencil");
        CC_BREAK_IF(!pencil);
        pencil->setPosition(CCPointMake(98.0f, 36.0f));
        addChild(pencil,0,kTagPencilButton);
        
        // initialize label
        CCLabelTTF* turnTitle = CCLabelTTF::labelWithString("回合", "Arial-BoldMT", 18.0f);
        CC_BREAK_IF(!turnTitle);
        turnTitle->setColor(ccBLUE);
        turnTitle->setPosition(CCPointMake(30.0f, 50.0f));
        addChild(turnTitle);
        
        // 
        CCLabelTTF* turn = CCLabelTTF::labelWithString("0", "Arial-BoldMT", 24.0f);
        turn->setColor(ccBLUE);
        turn->setPosition(CCPointMake(30.0f,20.0f));
        addChild(turn,0,kTagTurnLabel);
        
        //
        CCLabelTTF* state = CCLabelTTF::labelWithString("等待中", "Arial-BoldMT", 18.0f);
        state->setColor(ccGRAY);
        state->setPosition(CCPointMake(180.0f, 40.0f));
        addChild(state,0,kTagStateLabel);
        
        CCLabelTTF* name = CCLabelTTF::labelWithString("username", "Arial", 14.0f);
        name->setColor(ccRED);
        name->setPosition(CCPointMake(170.0f, 16.0f));
        addChild(name,0,kTagNameLabel);
                                                        
                                                        
        
        
        result = true;
    } while (0);
    return result;
}

void GameLabelNode::setGame(const Game *game) {
    gameId_ = game->objectId();
    // update turn
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << (game->turn() + 1);
    static_cast<CCLabelTTF*>(getChildByTag(kTagTurnLabel))->setString(oss.str().c_str());
    // state
    static_cast<CCLabelTTF*>(getChildByTag(kTagStateLabel))->setString(game->stateString().c_str());
    static_cast<CCLabelTTF*>(getChildByTag(kTagNameLabel))->setString(game->otherPlayerName().c_str());
    
    getChildByTag(kTagPencilButton)->setIsVisible(game->isMyTurn());
}

bool GameLabelNode::onTouchBegan(const cocos2d::CCPoint &localPos) {
    bool handled = false;
    trackingNode_ = hitTest(localPos);
    if(trackingNode_) {
        trackingNode_->setScale(0.9f);
        handled = true;
    }
    return handled;
}

void GameLabelNode::onTouchEnded(const cocos2d::CCPoint &localPos) {
    if(trackingNode_) {
        CCNode* node = hitTest(localPos);
        if(node == trackingNode_) {
            // on game clicked
            onNodeClicked(node);
        }
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

void GameLabelNode::onTouchMoved(const cocos2d::CCPoint &localPos) {
    if(trackingNode_) {
        if(hitTest(localPos) != trackingNode_) {
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        }
    }
}

void GameLabelNode::onTouchCancelled(const cocos2d::CCPoint &localPos) {
    if(trackingNode_) {
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

CCNode* GameLabelNode::hitTest(const cocos2d::CCPoint &localPos) {
    CCNode* node = NULL;
    Game* game = findGame();
    if(game && game->isMyTurn()) {
        CCNode* pencil = getChildByTag(kTagPencilButton);
        if(CCRect::CCRectContainsPoint(pencil->boundingBox(), localPos) && pencil->getIsVisible()) {
            node = pencil;
        }
    }
    return node;
}

Game* GameLabelNode::findGame() const {
    return UserProfile::sharedUserProfile()->findGame(gameId_);
}

void GameLabelNode::onNodeClicked(CCNode* button) {
    LobbyScene* ls = static_cast<LobbyScene*>(getParent()->getParent());
    ls->logic()->playGame(gameId_);
}

