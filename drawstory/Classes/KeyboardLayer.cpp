//
//  KeyboardLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "KeyboardLayer.h"

using namespace cocos2d;

bool KeyboardLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("keyboard");
        CC_BREAK_IF(!background);
        background->setPosition(CCPointMake(winSize.width * 0.5f, background->getContentSize().height * 0.5f));
        addChild(background);
        
        result = true;
    } while (0);
    return result;
}
