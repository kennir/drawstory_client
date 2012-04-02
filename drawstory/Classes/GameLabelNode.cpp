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

using namespace cocos2d;

enum { kTagTurnLabel = 1000,kTagStateLabel,kTagNameLabel };

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
        addChild(background);
        
        CCSprite* pencil = CCSprite::spriteWithSpriteFrameName("ui_pencil");
        CC_BREAK_IF(!pencil);
        pencil->setPosition(CCPointMake(-40.0f, 0.0f));
        addChild(pencil);
        
        // initialize label
        CCLabelTTF* turnTitle = CCLabelTTF::labelWithString("回合", "Arial-BoldMT", 18.0f);
        CC_BREAK_IF(!turnTitle);
        turnTitle->setColor(ccBLUE);
        turnTitle->setPosition(CCPointMake(-100.0f, 20.0f));
        addChild(turnTitle);
        
        // 
        CCLabelTTF* turn = CCLabelTTF::labelWithString("0", "Arial-BoldMT", 24.0f);
        turn->setColor(ccBLUE);
        turn->setPosition(CCPointMake(-100.0f,-10.0f));
        addChild(turn,0,kTagTurnLabel);
        
        //
        CCLabelTTF* state = CCLabelTTF::labelWithString("等待中", "Arial-BoldMT", 18.0f);
        state->setColor(ccGRAY);
        state->setPosition(CCPointMake(20.0f, 20.0f));
        addChild(state,0,kTagStateLabel);
        
        CCLabelTTF* name = CCLabelTTF::labelWithString("username", "Arial", 12.0f);
        name->setColor(ccRED);
        name->setPosition(CCPointMake(20.0f, -10.0f));
        addChild(name,0,kTagNameLabel);
                                                        
                                                        
        setContentSize(background->getContentSize());
        
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

    
}


