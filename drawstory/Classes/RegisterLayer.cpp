//
//  InputEmailLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "RegisterLayer.h"
#include "LobbyScene.h"
#include "LobbySceneLogic.h"
#include "UserProfile.h"

enum {
    kTagEmail       = 1000,
    kTagUsername,
}; 

enum { 
    kMaximumCharacterEmail = 32,
    kMaximumCharacterUsername = 8,
};


RegisterLayer::RegisterLayer()
: level_(NULL)
, trackingButton_(NULL)
{
    
}

RegisterLayer::~RegisterLayer()
{
    CCLOG("RegisterLayer::~RegisterLayer()");
    delete level_;
}

bool RegisterLayer::init()
{
    bool result = false;
    do {
        CC_BREAK_IF(!TextInputDelegate::init()); 
        
        // initialize level helper
        level_ = new LevelHelperLoader("registerlayer.plhs");
        CC_BREAK_IF(!level_);
        level_->addObjectsToWorld(NULL, this);
        
        // create email text filed
//        CCTextFieldTTF* field = addTextField(CCPointMake(60.0f, 308.0f), kTagEmail, "输入邮件地址登陆", "Thonburi", 14);
        CCTextFieldTTF* field = addTextField(CCPointMake(160.0f, 308.0f), kTagEmail, "输入邮件地址登陆", "Thonburi", 14);
//        field->setAnchorPoint(CCPointMake(0.0f, 0.5f));
        field->setColor(ccBLUE);
        // create user name text field
        field = addTextField(CCPointMake(160.0f, 221.0f), kTagUsername, "输入用户名(推荐)",  "Thonburi", 14);
//        field->setAnchorPoint(CCPointMake(0.0f, 0.5f));
        field->setColor(ccBLACK);
        
        result = true;
    } while (0);
    
    return result;
}

void RegisterLayer::onEnter()
{
    TextInputDelegate::onEnter();
    
    UserProfile* user = UserProfile::sharedUserProfile();
    if(!user->email().empty())
        setTextFieldString(kTagEmail, user->email().c_str());

}


void RegisterLayer::onTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    trackingButton_ = hitTestWithButton(convertTouchToNodeSpace(touch));
    if(trackingButton_ != NULL)
        trackingButton_->setScale(0.9f);
}

void RegisterLayer::onTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(trackingButton_)
    {
        LHSprite* node = hitTestWithButton(convertTouchToNodeSpace(touch));
        if(node != trackingButton_)
        {
            trackingButton_->setScale(1.0f);
            trackingButton_ = NULL;
        }
    }
}

void RegisterLayer::onTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(trackingButton_)
    {
        trackingButton_->setScale(1.0f);
        trackingButton_ = NULL;
        
        // start http connection
        LobbyScene* parent = static_cast<LobbyScene*>(getParent());
        parent->logic()->registerUser(getTextFieldString(kTagEmail), getTextFieldString(kTagUsername));
    }    
}

void RegisterLayer::onTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(trackingButton_)
    {
        trackingButton_->setScale(1.0f);
        trackingButton_ = NULL;
    }
}

int RegisterLayer::getMaximumCharacters(cocos2d::CCTextFieldTTF *textField) const
{
    int maxChar = kDefaultMaximumCharacter;
    switch (textField->getTag()) {
        case kTagEmail:
            maxChar = kMaximumCharacterEmail;
            break;
        case kTagUsername:
            maxChar = kMaximumCharacterUsername;
            break;
        default:
            break;
    }
    return maxChar;
}

CCRect RegisterLayer::getTextFieldRect(cocos2d::CCTextFieldTTF *textField) const
{
    CCRect rect = textField->boundingBox();
    switch (textField->getTag()) {
        case kTagEmail:
            rect = level_->spriteWithUniqueName("textfield_email")->boundingBox();
            break;
        case kTagUsername:
            rect = level_->spriteWithUniqueName("textfield_username")->boundingBox();
            break;
        default:
            break;
    }
    
    return rect;
}


LHSprite* RegisterLayer::hitTestWithButton(const cocos2d::CCPoint &localPos)
{
    LHSprite* node = NULL;
    
    LHSprite* startButton = level_->spriteWithUniqueName("email_start");
    CC_ASSERT(startButton);
    
    if(CCRect::CCRectContainsPoint(startButton->boundingBox(), localPos))
        node = startButton;
    
    return node;
}

