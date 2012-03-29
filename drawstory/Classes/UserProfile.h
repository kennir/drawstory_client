//
//  UserProfile.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_UserProfile_h
#define drawstory_UserProfile_h

#include "cocos2d.h"
#include <string>


class UserProfile
{
public:
    static UserProfile* sharedUserProfile();
    static void purgeUserProfile();
    
    // load user profile from user default
    bool init();
    
    const std::string& email() const { return email_; }
    const std::string& password() const { return password_; }
    
    void setEmail(const std::string& newEmail);
    void setPassword(const std::string& newPassword);
protected:
    std::string email_;
    std::string password_;
};



#endif
