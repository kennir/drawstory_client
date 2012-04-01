//
//  MessageLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-31.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_MessageLayer_h
#define drawstory_MessageLayer_h

#include "cocos2d.h"
#include "LevelHelperLoader.h"

class MessageLayer : public cocos2d::CCLayer
{
public:
    static MessageLayer* layerWithLabel(const std::string& label,
                                        bool showCancelButton,
                                        cocos2d::CCObject* delegate,
                                        cocos2d::SEL_CallFunc selector,
                                        int msgTag);
    LAYER_NODE_FUNC(MessageLayer)
public:
    MessageLayer();
    virtual ~MessageLayer();
    
    virtual bool init();
    void setLabel(const std::string& label);
    void setCancelButton(bool showCancelButton,cocos2d::CCObject* delegate,cocos2d::SEL_CallFunc selector);
    void setMessageTag(int tag) { messageTag_ = tag; }
    int messageTag() const { return messageTag_; }
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
protected:
    
    LHSprite* hitTest(const cocos2d::CCPoint& localPos) const;
protected:
    LevelHelperLoader* level_;
    cocos2d::CCLabelTTF* label_;
    
    LHSprite* trackingNode_;
    
    bool cancelButtonVisibled_;
    cocos2d::CCObject* delegate_;
    cocos2d::SEL_CallFunc selector_;
    
    int messageTag_;
};

#endif
