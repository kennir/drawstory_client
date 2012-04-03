//
//  TitleBarLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "TitleBarLayer.h"

using namespace cocos2d;


bool TitleBarLayer::init() { 
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        //== top bar
        CCSprite* bannerup = CCSprite::spriteWithSpriteFrameName("banner_up");
        CC_BREAK_IF(!bannerup);
        bannerup->setPosition(CCPointMake(winSize.width * 0.5f, 
                                          bannerup->getContentSize().height * 0.5f));
        addChild(bannerup);
        
        result = true;
    } while (0);
    return result;
}

