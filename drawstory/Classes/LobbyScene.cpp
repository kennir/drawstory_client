//
//  LobbyScene.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LobbyScene.h"

using namespace cocos2d;

CCScene* LobbyScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = LobbyScene::node();
    scene->addChild(layer);
    
    return scene;
}


bool LobbyScene::init()
{
    bool result = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());
        // load level's
        level_ = new LevelHelperLoader("lobbyscene.plhs");
        CC_BREAK_IF(!level_);
        level_->addObjectsToWorld(NULL, this);
        
        
        
        result = true;
    } while (0);
    return result;
}






