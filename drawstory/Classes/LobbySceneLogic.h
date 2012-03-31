//
//  LobbySceneLogic.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_LobbySceneLogic_h
#define drawstory_LobbySceneLogic_h

#include "SimpleHttpRequest.h"

typedef enum 
{
    kLobbyStateUnhandle,
    kLobbyStateRegisterUser,
    kLobbyStateWaitingForRegisterUser,
    kLobbyStateLoginUser,
    kLobbyStateWaitingForLoginUser,
    kLobbyStateIdle,
    kLobbyStateWaitingForRandomGame,
} LobbyState;


class LobbyScene;
class LobbySceneLogic : public SimpleHttpRequestDelegate
{
public:
    LobbySceneLogic(LobbyScene* scene);
    virtual ~LobbySceneLogic();
    
    bool init();
    
    void registerUser(const std::string& email,const std::string& username);
    // get email and password from user default
    void loginUser();
    
    void onResponse(bool result,const Json::Value& response);
    
    void setCurrentState(LobbyState newState);
    LobbyState currentState() const { return state_; }
protected:
    void processResponseForRegisterUser(bool curlState,const Json::Value& response);
    void processResponseForLoginUser(bool curlState,const Json::Value& response);
protected:
    LobbyScene* scene_;
    LobbyState state_;
    
    SimpleHttpRequest* currentRequest_;
};

#endif
