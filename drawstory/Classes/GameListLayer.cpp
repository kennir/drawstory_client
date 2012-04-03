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

using namespace cocos2d;

enum { kZButton = 1, kZGameLabel = 1 };
enum { kTagStartGameButton,kTagGameLabel = 100 };


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

        
        result = true;
    } while (0);
    return result;
}


void GameListLayer::synchronizeGameList() {
    removeAllNodes();
    
    CCPoint position = CCPointMake(0,-132.0f - 33.5f);
        
    UserProfile* user = UserProfile::sharedUserProfile();
    const std::list<Game*>& games = user->games();
    std::list<Game*>::const_iterator it = games.begin();
    for(; it != games.end(); ++it){
        GameLabelNode* label = createLabelNode();
        label->setPosition(position);
        
        label->setGame(*it);
        
        addChild(label,kZGameLabel,kTagGameLabel);
        position.y -= label->getContentSize().height;
    }
    
    
    setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width,
                              70.0f * games.size()));
}

void GameListLayer::removeAllNodes() {
    std::list<GameLabelNode*>::iterator it = nodes_.begin();
    while(it != nodes_.end()) {
        pool_.push(*it);
        removeChild(*it, false);
        it = nodes_.erase(it);
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
    }
    return node;
}

