//
//  CharacterBox.cpp
//  drawstory
//
//  Created by 靖宇 张 on 12-4-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CharacterBox.h"

using namespace cocos2d;


CharacterBox* CharacterBox::characterBoxWithSpriteFrameName(const char *frameName) {
    CharacterBox* cb = new CharacterBox;
    if(cb->initWithSpriteFrameName(frameName))
        cb->autorelease();
    else {
        delete cb;
        cb = NULL;
    }
    return cb;
}

void CharacterBox::setCharacter(char ch) {
    if(ch != character_) {
        if(ch == ' ') {
            CCSprite* sprite = (CCSprite*)getChildByTag(kTagCharacter);
            if(sprite)
                removeChild(sprite, true);
        } else {
            std::ostringstream oss;
            oss << "b_" << ch;
            CCSprite* sprite = CCSprite::spriteWithSpriteFrameName(oss.str().c_str());
            
            CCSize parentSize = getContentSize();
            sprite->setPosition(CCPointMake(parentSize.width * 0.5f, parentSize.height * 0.5f));
            addChild(sprite,0,kTagCharacter);
        }
        
        character_ = ch;
    }
}




