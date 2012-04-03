//
//  ToggleButton.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ToggleButton.h"

using namespace cocos2d;

enum { kTagNormalSprite = 1, kTagToggledSprite };

ToggleButton* ToggleButton::toggleButtonWithImage(const char *normalImage, const char *toggledImage) {
    ToggleButton* tb = new ToggleButton;
    if(tb && tb->initWithImage(normalImage, toggledImage)) {
        tb->autorelease();
    } else {
        delete tb;
        tb = NULL;
    }
    return tb;
}

ToggleButton::~ToggleButton() {
    CC_SAFE_RELEASE(normalFrame_);
    CC_SAFE_RELEASE(toggledFrame_);
}

bool ToggleButton::initWithImage(const char *normalImage, const char *toggledImage) {
    bool result = false;
    do {        
        CC_BREAK_IF(!CCSprite::init());
        
        // get frame
        normalFrame_ = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(normalImage);
        CC_BREAK_IF(!normalFrame_);
        normalFrame_->retain();
        setDisplayFrame(normalFrame_);
        
        toggledFrame_ = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(toggledImage);
        CC_BREAK_IF(!toggledFrame_);
        toggledFrame_->retain();

        toggled_ = false;
    
        result = true;
    } while (0);
    return result;
}

bool ToggleButton::toggle() {
    toggled_ = !toggled_;
    setDisplayFrame((toggled_) ? toggledFrame_ : normalFrame_);
    return toggled();
}

void ToggleButton::setToggled(bool t) {
    if(toggled_ != t) {
        toggle();
    }
}
