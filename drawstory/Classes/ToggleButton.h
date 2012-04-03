//
//  ToggleButton.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_ToggleButton_h
#define drawstory_ToggleButton_h

#include "cocos2d.h"

class ToggleButton : public cocos2d::CCSprite{
public:
    static ToggleButton* toggleButtonWithImage(const char* normalImage,const char* toggledImage);
    virtual ~ToggleButton();
public:
    bool initWithImage(const char* normalImage,const char* toggledImage);
    
    bool toggled() const { return toggled_; }
    void setToggled(bool t);
    bool toggle();
    
    
protected:
    bool toggled_;
    
    cocos2d::CCSpriteFrame* normalFrame_;
    cocos2d::CCSpriteFrame* toggledFrame_;
    
    cocos2d::CCObject* delegate_;
    cocos2d::SEL_CallFunc selector_;
};



#endif
