//
//  MessageLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-31.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MessageLayer.h"

using namespace cocos2d;


MessageLayer* MessageLayer::layerWithLabel(const std::string &label,                                        
                                           bool showCancelButton,
                                           CCObject* delegate,
                                           SEL_CallFunc selector,
                                           int msgTag)
{
    MessageLayer* layer = MessageLayer::node();
    if(layer){
        layer->setLabel(label);
        layer->setCancelButton(showCancelButton,delegate,selector);
        layer->setMessageTag(msgTag);
    }
    return layer;
}


MessageLayer::MessageLayer()
: level_(NULL)
, label_(NULL)
, trackingNode_(NULL)
, cancelButtonVisibled_(true)
, delegate_(NULL)
, selector_(NULL)
, messageTag_(0)
{
    
}

MessageLayer::~MessageLayer()
{
    delete level_;
}


bool MessageLayer::init()
{
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        level_ = new LevelHelperLoader("messagelayer.plhs");
        CC_BREAK_IF(!level_);
        level_->addObjectsToWorld(NULL, this);
        
        // create label
        label_ = CCLabelTTF::labelWithString("", 
                                             CCSizeMake(320, 480), 
                                             CCTextAlignmentCenter, 
                                             "Verdana", 
                                             16.0f);
        label_->setPosition(CCPointMake(160,240));
        addChild(label_);

        
        result = true;
    } while (0);
    
    return result;
}

void MessageLayer::setLabel(const std::string &label)
{
    label_->setString(label.c_str());
}

void MessageLayer::setCancelButton(bool showCancelButton,cocos2d::CCObject* delegate,cocos2d::SEL_CallFunc selector)
{
    if(showCancelButton != cancelButtonVisibled_)
    {
        LHSprite* button = level_->spriteWithUniqueName("ui_button_cancel");
        button->setIsVisible(showCancelButton);
        
        cancelButtonVisibled_ = showCancelButton;
    }
    
    delegate_ = delegate;
    selector_ = selector;
}

void MessageLayer::onEnter()
{
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1, true);
}

void MessageLayer::onExit()
{
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}


bool MessageLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    trackingNode_ = hitTest(convertTouchToNodeSpace(touch));
    if(trackingNode_)
        trackingNode_->setScale(0.9f);
    
    return true;
}

void MessageLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_) {
        LHSprite* node = hitTest(convertTouchToNodeSpace(touch));
        if(node != trackingNode_) {
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        }
    }
}

void MessageLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_) {
        LHSprite* node = hitTest(convertTouchToNodeSpace(touch));
        if(node == trackingNode_ && selector_ && delegate_) {
            (delegate_->*selector_)();
        }
        
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

void MessageLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_) {
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}


LHSprite* MessageLayer::hitTest(const cocos2d::CCPoint &localPos) const 
{
    LHSprite* node = NULL;
    if(cancelButtonVisibled_)
    {
        LHSprite* cancelButton = level_->spriteWithUniqueName("ui_button_cancel");
        if(CCRect::CCRectContainsPoint(cancelButton->boundingBox(), localPos))
            node = cancelButton;
    }
    return node;
}


