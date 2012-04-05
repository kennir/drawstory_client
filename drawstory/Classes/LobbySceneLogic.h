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
#include <list>
#include "types.h"



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
    // create a random game
    void createRandomGame();
    void cancelCreateRandomGame();
    void queryCurrentRandomGame();
    void cancelQueryCurrentRandomGame();
    void queryGamesForUser();
    void playGame(const std::string& gameId);
    void sendPainting(const std::string& painting);
    void sendPaintingRequestForAllGames();
                      
    void onResponse(bool result,const Json::Value& response,SimpleHttpRequest* request);
    
    void setCurrentState(LobbyState newState);
    LobbyState currentState() const { return state_; }
    LobbyState previousState() const { return previousState_; }
    
    bool hasRunningRequest() const { return !runningRequests_.empty(); }
protected:
    void processResponseForRegisterUser(bool curlState,const Json::Value& response);
    void processResponseForLoginUser(bool curlState,const Json::Value& response);
    void processResponseForCreateRandomGame(bool curlState,const Json::Value& response);
    void processResponseForQueryCurrentRandomGame(bool curlState,const Json::Value& response);
    void processResponseForQueryGamesForUser(bool curlState,const Json::Value& response);
    void processResponseForSendPainting(bool curlState,const Json::Value& response);
    
    void addReqeust(SimpleHttpRequest* request);
    void cancelAllRequestsForTag(int tag);
protected:
    LobbyScene* scene_;
    LobbyState previousState_;
    LobbyState state_;
    
    std::list<SimpleHttpRequest*> runningRequests_;
    pthread_mutex_t requestMutex_;
    
    bool logined_;
    
    std::string objectIdForCurrentGameId_;
};

#endif
