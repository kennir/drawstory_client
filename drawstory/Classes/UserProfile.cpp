//
//  UserProfile.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "UserProfile.h"

using namespace cocos2d;

#pragma mark static variables

static UserProfile* _userProfile = NULL;


#pragma mark member functions

UserProfile* UserProfile::sharedUserProfile()
{
    if(!_userProfile)
    {
        _userProfile = new UserProfile;
        if(!_userProfile->init())
        {
            CC_SAFE_DELETE(_userProfile);
        }
    }
    
    return _userProfile;
}


void UserProfile::purgeUserProfile()
{
    CC_SAFE_DELETE(_userProfile);
}

bool UserProfile::init()
{    
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    if(!userDefault)
    {
        CCLOGERROR("Can't load user default");
        return false;
    }
    
    // get email address
    email_ = userDefault->getStringForKey("email");
    password_ = userDefault->getStringForKey("password");
    
    return true;
}


void UserProfile::setEmail(const std::string &newEmail)
{
    if(email_ != newEmail)
    {
        email_ = newEmail;
        
        CCUserDefault* ud = CCUserDefault::sharedUserDefault();
        ud->setStringForKey("email", email_);
        ud->flush();
    }
}

void UserProfile::setPassword(const std::string &newPassword)
{
    if(password_ != newPassword)
    {
        password_ = newPassword;
        
        CCUserDefault* ud = CCUserDefault::sharedUserDefault();
        ud->setStringForKey("password", password_);
        ud->flush();
    }
}
