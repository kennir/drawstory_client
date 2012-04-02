//
//  GameLabelLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameLabelLayer.h"

using namespace cocos2d;

GameLabelLayer::GameLabelLayer() : level_(NULL) {
    
}

GameLabelLayer::~GameLabelLayer(){
    delete level_;
}

bool GameLabelLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        // Initialize level helpre
        level_ = new LevelHelperLoader("gamelabel.plhs");
        CC_BREAK_IF(!level_);
        level_->addObjectsToWorld(NULL,this);
        
        // add label of turn
        CCLabelTTF* turnTitleLabel = CCLabelTTF::labelWithString("回合","AppleGothic",14.0f);
        turnTitleLabel->setPosition(CCPointMake(30.0f,55.0f));
        addChild(turnTitleLabel);
        
        
        
        result = true;
    } while (0);
    return result;
}

