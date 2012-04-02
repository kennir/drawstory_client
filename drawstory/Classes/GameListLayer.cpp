//
//  GameListLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameListLayer.h"
#include "GameLabelLayer.h"
#include "UserProfile.h"
#include "Game.h"

bool GameListLayer::init(){
    return CCLayer::init();
}


void GameListLayer::synchronizeGameList(){
    removeAllLabels();
    
    CCPoint pos(CCPointMake(0.0f,0.0f));
    
    std::list<Game*> games = UserProfile::sharedUserProfile()->games();
    std::list<Game*>::const_iterator it = games.begin();
    for(; it != games.end(); ++it){
//        Game* game = (*it);
        
        GameLabelLayer* label = createLabel();
        addLabel(pos, label);
    }
}


void GameListLayer::addLabel(const cocos2d::CCPoint &pos, GameLabelLayer *label){
    label->setPosition(pos);
    usedLabel_.push_back(label);
    addChild(label);
}

void GameListLayer::removeAllLabels(){
    std::list<GameLabelLayer*>::iterator it = usedLabel_.begin();
    while(it != usedLabel_.end()){
        removeChild(*it, false);
        labelPool_.push(*it);
        it = usedLabel_.erase(it);
    }
}

GameLabelLayer* GameListLayer::createLabel() {
    GameLabelLayer* label = NULL;
    if(!labelPool_.empty()) {
        label = labelPool_.top();
        labelPool_.pop();
    } else {
        label = GameLabelLayer::node();
        CC_ASSERT(label != NULL);
        label->retain();
    }
    
    return label;
}

