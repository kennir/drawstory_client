//
//  LobbySceneLogic.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LobbySceneLogic.h"
#include "LobbyScene.h"
#include "UserProfile.h"


static const char* _hostURL = "http://192.168.254.101:3000/";

LobbySceneLogic::LobbySceneLogic(LobbyScene* scene)
: scene_(scene)
, state_(kLobbyStateUnhandle)
, currentRequest_(NULL)
{    
}

LobbySceneLogic::~LobbySceneLogic()
{
    
}

bool LobbySceneLogic::init()
{
    UserProfile* user = UserProfile::sharedUserProfile();
    if(user->email().empty() || user->password().empty())
        setCurrentState(kLobbyStateRegisterUser);
    else 
    {
        loginUser();
    }
    
    return true;
}


void LobbySceneLogic::setCurrentState(LobbyState newState)
{
    if(state_ != newState)
    {
        state_ = newState;
        scene_->logicChanged();
    }
}

void LobbySceneLogic::onResponse(bool result, const Json::Value& response)
{
    CC_SAFE_RELEASE_NULL(currentRequest_);
    
    switch(currentState()){
        case kLobbyStateWaitingForRegisterUser:
            processResponseForRegisterUser(result,response);
            break;
        case kLobbyStateWaitingForLoginUser:
            processResponseForLoginUser(result, response);
            break;
        default:
            break;
    }


    
}


void LobbySceneLogic::registerUser(const std::string &email, const std::string &username)
{
//    CCLOG("LobbySceneLogic::registerUser: email:%s username:%s",email.c_str(),username.c_str());
    
    CC_ASSERT(currentState() == kLobbyStateRegisterUser);
    
    // make URL
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "register";
    CCLOG("LobbySceneLogic::registerUser URL:%s",url.str().c_str());
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "email=" << email << "&username=" << username;
    CCLOG("LobbySceneLogic::registerUser body:%s",body.str().c_str());
    
    currentRequest_ = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                  SimpleHttpRequest::kHttpMethodPost, 
                                                                  this);
    currentRequest_->setPostField(body.str());
    currentRequest_->retain();
    currentRequest_->start();
    setCurrentState(kLobbyStateWaitingForRegisterUser);
}


void LobbySceneLogic::loginUser()
{
    
    UserProfile* user = UserProfile::sharedUserProfile();  
    // make URL
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "login";
    CCLOG("LobbySceneLogic::registerUser URL:%s",url.str().c_str());
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "email=" << user->email() << "&password=" << user->password();
    CCLOG("LobbySceneLogic::registerUser body:%s",body.str().c_str());
    
    currentRequest_ = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                  SimpleHttpRequest::kHttpMethodPost, 
                                                                  this);
    currentRequest_->setPostField(body.str());
    currentRequest_->retain();
    currentRequest_->start();
    setCurrentState(kLobbyStateWaitingForLoginUser);
    
}


void LobbySceneLogic::processResponseForRegisterUser(bool curlState,const Json::Value &response)
{
    if(!curlState)
    {
        CCLOG("LobbySceneLogic::processResponseForRegisterUser:curl failed:%d",response["reason"].asInt());
        setCurrentState(kLobbyStateRegisterUser);
    }
    else 
    {
        bool result = response["result"].asBool();
        if(result == false)
        {
            CCLOG("LobbySceneLogic::processResponseForRegisterUser: Register failed:%d",response["reason"].asInt());
            setCurrentState(kLobbyStateRegisterUser);
        }
        else
        {
            std::string email = response["email"].asString();
            std::string password = response["password"].asString();
            CCLOG("LobbySceneLogic::processResponseForRegisterUser: Register successed, Email:%s Password:%s",email.c_str(),password.c_str());
            
            // write to user default
            UserProfile* user = UserProfile::sharedUserProfile();
            user->setEmail(email);
            user->setPassword(password);
            
            
            setCurrentState(kLobbyStateLoginUser);
        }
    }
}


void LobbySceneLogic::processResponseForLoginUser(bool curlState, const Json::Value &response)
{
    if(!curlState)
    {
        CCLOG("LobbySceneLogic::processResponseForLoginUser:curl failed:%d",response["reason"].asInt());
        setCurrentState(kLobbyStateRegisterUser);
    }
    else 
    {
        bool result = response["result"].asBool();
        if(result == false)
        {
            CCLOG("LobbySceneLogic::processResponseForLoginUser: login failed:%d",response["reason"].asInt());
            setCurrentState(kLobbyStateRegisterUser);
        }
        else
        {
            CCLOG("LobbySceneLogic::processResponseForLoginUser: login successed");
            setCurrentState(kLobbyStateIdle);
        }
    }
}



