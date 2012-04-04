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
#include "Game.h"
#include "list"
#include "Json/json.h"

class UserProfile
{
public:
    static UserProfile* sharedUserProfile();
    static void purgeUserProfile();
    
public:
    virtual ~UserProfile();
    
    // load user profile from user default
    bool init();
    
    const std::string& email() const { return email_; }
    const std::string& objectId() const { return objectId_; }
    const std::string& password() const { return password_; }
    
    void setEmail(const std::string& newEmail);
    void setObjectId(const std::string& newObjId);
    void setPassword(const std::string& newPassword);
    
    void synchronizeGameList(const Json::Value& games);
    

    const std::list<Game*>& games() const { return games_; }
    Game* findGame(const std::string& gameId) const;
protected:
    std::string email_;
    std::string objectId_;
    std::string password_;
    
    std::list<Game*> games_;
};



#endif
