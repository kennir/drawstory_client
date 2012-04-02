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

enum { kZGameLabel = 1 };
enum { kTagGameLabel = 100 };


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
        
        result = true;
    } while (0);
    return result;
}


void GameListLayer::synchronizeGameList() {
    removeAllNodes();
    
    CCPoint position = CCPointMake(0,-35.0f);
        
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

