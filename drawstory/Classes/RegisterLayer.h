//
//  RegisterLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_RegisterLayer_h
#define drawstory_RegisterLayer_h

#include "cocos2d.h"
#include "TextInputDelegate.h"
#include "types.h"


class RegisterLayer : public TextInputDelegate
{
public:
    LAYER_NODE_FUNC(RegisterLayer)
public:
    RegisterLayer();
    virtual ~RegisterLayer();
    
    virtual bool init();
protected:    
    virtual void onEnter();
    virtual void onTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void onTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void onTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void onTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);   
    
    virtual int touchPriority() const { return kTouchPriorityRegisterLayer; }
    virtual cocos2d::CCRect getTextFieldRect(cocos2d::CCTextFieldTTF* textField);
    virtual int getMaximumCharacters(cocos2d::CCTextFieldTTF* textField) const;
    
protected:
    cocos2d::CCSprite* hitTestWithButton(const cocos2d::CCPoint& localPos);
protected:
    cocos2d::CCSprite* trackingButton_;
};


#endif
