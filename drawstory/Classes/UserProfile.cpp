//
//  UserProfile.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "UserProfile.h"

using namespace cocos2d;


#pragma mark static functions

struct FindGameById
{
    FindGameById(const char* _id) : id(_id) { }

    bool operator()(const Game* game) const {
        return (strcmp(game->objectId().c_str(), id) == 0);
    }
                 
    const char* id;
};


static void removeAllGames(std::list<Game*>& games){
    std::list<Game*>::iterator it = games.begin();
    for(; it != games.end(); ++it)
        delete (*it);
    games.clear();
}

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

UserProfile::~UserProfile(){
    removeAllGames(games_);
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
    objectId_ = userDefault->getStringForKey("objectid");
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

void UserProfile::setObjectId(const std::string &newObjId)
{
    if(objectId_ != newObjId)
    {
        objectId_ = newObjId;
        
        CCUserDefault* ud = CCUserDefault::sharedUserDefault();
        ud->setStringForKey("objectid", objectId_);
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

void UserProfile::synchronizeGameList(const Json::Value &games) {
    std::list<Game*> temporary;
    std::swap(temporary, games_);
    CC_ASSERT(games_.empty());
    
    std::string gid;    // for cache
    for(int idx = 0; idx < games.size(); ++idx){
        const Json::Value& game = games[idx];
        const Json::Value& detail = game["detail"];
        
        gid = detail["_id"].asString();
        std::list<Game*>::iterator it = std::find_if(temporary.begin(),temporary.end(),FindGameById(gid.c_str()));
        if(it == temporary.end()){
            // Insert a game to list
            Game* ng = Game::gameFromJson(game);
            CC_ASSERT(ng != NULL);
            // push game to list
            games_.push_back(ng);
        } else {
            // Update game and remove to games_
            (*it)->updateFromJson(game);
            games_.push_back(*it);
            temporary.erase(it);
        }   
    }
    
    CCLOG("UserProfile::synchronizeGameList:%d games synchronized. %d games removed.",games_.size(),temporary.size());
    
    // remove all games still in the temporary
    removeAllGames(temporary);
}

Game* UserProfile::findGame(const std::string &gameId) const {
    std::list<Game*>::const_iterator it = std::find_if(games_.begin(), games_.end(), FindGameById(gameId.c_str()));
    return (it != games_.end()) ? (*it) : NULL;
}

    