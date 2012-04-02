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

using namespace cocos2d;


enum {
    kZUILabel = 1,
    kZUITextField = 2,
};

enum {
    kTagEmail       = 1000,
    kTagEmailBG,
    kTagUsername,
    kTagUsernameBG,
    kTagStartButton,
}; 

enum { 
    kMaximumCharacterEmail = 32,
    kMaximumCharacterUsername = 8,
};


RegisterLayer::RegisterLayer()
: trackingButton_(NULL)
{
    
}

RegisterLayer::~RegisterLayer()
{
    CCLOG("RegisterLayer::~RegisterLayer()");

}

bool RegisterLayer::init()
{
    bool result = false;
    do {
        CC_BREAK_IF(!TextInputDelegate::init()); 
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // add background
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("email_background");
        background->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height * 0.5f));
        CC_ASSERT(background);
        addChild(background);
        
        
        CCPoint emailTextFieldPos = CCPointMake(158.0f, 308.0f);
        CCSprite* emailbg = CCSprite::spriteWithSpriteFrameName("textfield");
        emailbg->setPosition(emailTextFieldPos);
        addChild(emailbg,kZUILabel,kTagEmailBG);
        CCTextFieldTTF* field = addTextField(emailTextFieldPos, kZUITextField,kTagEmail, "输入邮件地址登陆", "Thonburi", 14);
        field->setColor(ccBLUE);
        
        CCPoint nameTextFieldPos = CCPointMake(158.0f,221.0f);
        CCSprite* namebg = CCSprite::spriteWithSpriteFrameName("textfield");
        namebg->setPosition(nameTextFieldPos);
        addChild(namebg,kZUILabel,kTagUsernameBG);
        field = addTextField(nameTextFieldPos, kZUITextField, kTagUsername, "输入用户名(推荐)",  "Thonburi", 14);
        field->setColor(ccBLACK);
        
        CCSprite* startbutton = CCSprite::spriteWithSpriteFrameName("email_start");
        startbutton->setPosition(CCPointMake(160.0f, 180.0f));
        addChild(startbutton,kZUILabel,kTagStartButton);
        
        
        
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
        CCSprite* node = hitTestWithButton(convertTouchToNodeSpace(touch));
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

CCRect RegisterLayer::getTextFieldRect(cocos2d::CCTextFieldTTF *textField) {
    return static_cast<CCSprite*>(getChildByTag(textField->getTag() + 1))->boundingBox();    
}


CCSprite* RegisterLayer::hitTestWithButton(const cocos2d::CCPoint &localPos)
{
    CCSprite* node = NULL;
    
    CCSprite* startButton = static_cast<CCSprite*>(getChildByTag(kTagStartButton));
    CC_ASSERT(startButton);
    
    if(CCRect::CCRectContainsPoint(startButton->boundingBox(), localPos))
        node = startButton;
    
    return node;
}

