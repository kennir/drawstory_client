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
#include "GlobalData.h"


enum {
    kHttpRequestRegister,
    kHttpRequestLogin,
    kHttpRequestCreateRandomGame,
    kHttpRequestQueryCurrentRandomGame,
    kHttpRequestCancelQueryCurrentRandomGame,
    kHttpRequestQueryGamesForUser,
};

enum {
    kRandomGameCreate   = 1,
    kRandomGameJoin,
};

static const char* _hostURL = "http://192.168.254.101:3000/";

LobbySceneLogic::LobbySceneLogic(LobbyScene* scene)
: scene_(scene)
, previousState_(kLobbyStateUnhandle)
, state_(kLobbyStateUnhandle)
, logined_(false)
{    
    pthread_mutex_init(&requestMutex_,NULL);
}

LobbySceneLogic::~LobbySceneLogic()
{
    // Cancel all request
    pthread_mutex_lock(&requestMutex_);
    std::list<SimpleHttpRequest*>::iterator it = runningRequests_.begin();
    for(; it != runningRequests_.end(); ++it)
    {
        (*it)->cancel();
        (*it)->release();
    }
    runningRequests_.clear();
    pthread_mutex_unlock(&requestMutex_);
    
    pthread_mutex_destroy(&requestMutex_);
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
        CCLOG("STATE CHANGED:Old:%d New:%d",state_,newState);
        previousState_ = state_;
        state_ = newState;
        scene_->logicChanged();
    }
}

void LobbySceneLogic::onResponse(bool result, const Json::Value& response,SimpleHttpRequest* request)
{    
    switch(request->tag()) {
        case kHttpRequestRegister:
            CC_ASSERT(currentState() == kLobbyStateWaitingForRegisterUser);
            processResponseForRegisterUser(result,response);
            break;
        case kHttpRequestLogin:
            CC_ASSERT(currentState() == kLobbyStateWaitingForLoginUser);
            processResponseForLoginUser(result, response);
            break;
        case kHttpRequestCreateRandomGame:
            processResponseForCreateRandomGame(result, response);
            break;
        case kHttpRequestQueryCurrentRandomGame:
            processResponseForQueryCurrentRandomGame(result, response);
            break;
        case kHttpRequestCancelQueryCurrentRandomGame:
            CCLOG("kHttpRequestCancelQueryCurrentRandomGame: Ignored");
            break;
        case kHttpRequestQueryGamesForUser:
            processResponseForQueryGamesForUser(result, response);
            break;
        default:
            break;
    }

    // remove from list
    pthread_mutex_lock(&requestMutex_);
    std::list<SimpleHttpRequest*>::iterator it = std::find(runningRequests_.begin(), runningRequests_.end(), request);
    if(it != runningRequests_.end()) {
        (*it)->release();
        runningRequests_.erase(it);
    } else {
        CCLOG("Can't find request from list");
    }
    pthread_mutex_unlock(&requestMutex_);
}


void LobbySceneLogic::registerUser(const std::string &email, const std::string &username)
{
//    CCLOG("LobbySceneLogic::registerUser: email:%s username:%s",email.c_str(),username.c_str());
    
    CC_ASSERT(currentState() == kLobbyStateRegisterUser);
    
    // make URL
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "register";
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "email=" << email << "&username=" << username;
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodPost, 
                                                                             this,
                                                                             kHttpRequestRegister);
    request->setPostField(body.str());
    request->retain();
    addReqeust(request);
    request->start();

    setCurrentState(kLobbyStateWaitingForRegisterUser);
}


void LobbySceneLogic::loginUser()
{
    
    UserProfile* user = UserProfile::sharedUserProfile();  
    // make URL
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "login";
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "email=" << user->email() << "&password=" << user->password();
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodPost, 
                                                                             this,
                                                                             kHttpRequestLogin);
    request->setPostField(body.str());
    request->retain();
    addReqeust(request);
    request->start();

    setCurrentState(kLobbyStateWaitingForLoginUser);
}


void LobbySceneLogic::createRandomGame()
{
    CC_ASSERT(logined_);
    
    UserProfile* user = UserProfile::sharedUserProfile();
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "game/random";
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "email=" << user->email();
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodPost, 
                                                                             this,
                                                                             kHttpRequestCreateRandomGame);
    request->setPostField(body.str());
    request->retain();
    addReqeust(request);
    request->start();
    
    setCurrentState(kLobbyStateWaitingForCreateRandomGame);
}


void LobbySceneLogic::queryCurrentRandomGame()
{
    CC_ASSERT(!objectIdForCurrentGameId_.empty());
    
//    UserProfile* user = UserProfile::sharedUserProfile();
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "game/" << objectIdForCurrentGameId_;
        
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodGet, 
                                                                             this,
                                                                             kHttpRequestQueryCurrentRandomGame);
    
    request->retain();
    addReqeust(request);
    request->start();
}

void LobbySceneLogic::cancelQueryCurrentRandomGame() {
    cancelAllQueryRandomGameRequests();
    
    // notify server to remove game if not paired
//    UserProfile* user = UserProfile::sharedUserProfile();
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "game/" << objectIdForCurrentGameId_;
    
    std::stringstream body(std::stringstream::in | std::stringstream::out);
    body << "owner=" << UserProfile::sharedUserProfile()->objectId();
    
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodDelete, 
                                                                             this,
                                                                             kHttpRequestCancelQueryCurrentRandomGame);

    
    request->setPostField(body.str());
    request->retain();
    addReqeust(request);
    request->start();
    
    // set to idle state
    setCurrentState(kLobbyStateIdle);
}


void LobbySceneLogic::queryGamesForUser()
{
    UserProfile* user = UserProfile::sharedUserProfile();
    CC_ASSERT(!user->objectId().empty());
    
    std::stringstream url( std::stringstream::in | std::stringstream::out );
    url << _hostURL << "user/games/" << user->objectId();
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodGet, 
                                                                             this,
                                                                             kHttpRequestQueryGamesForUser);
    

    request->retain();
    addReqeust(request);
    request->start();
}

void LobbySceneLogic::cancelAllQueryRandomGameRequests() {
    pthread_mutex_lock(&requestMutex_);
    
    std::list<SimpleHttpRequest*>::iterator it = runningRequests_.begin();
    while( it != runningRequests_.end())
    {
        SimpleHttpRequest* req = (*it);
        if(req->tag() == kHttpRequestQueryCurrentRandomGame)
        {
            req->cancel();
            req->release();
            it = runningRequests_.erase(it);
        }
        else {
            ++it;
        }
    }
    
    pthread_mutex_unlock(&requestMutex_);
}

void LobbySceneLogic::playGame(const std::string &gameId) {
    Game* game = UserProfile::sharedUserProfile()->findGame(gameId);
    if(!game) {
        CCLOG("Can't find game for id:%s",gameId.c_str());
    } else {
        if(game->hasAnswerReplay()) {
            CCLOG("goto answer replay scene");
        } else if(game->hasQuestionReplay()) {
            CCLOG("goto answer scene");
        }
        
        GlobalData::sharedGlobalData()->setCurrentGameId(gameId);
        
        // goto paint scene
        setCurrentState(kLobbyStatePaintQuestion);
        
    }
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
            std::string objectId = response["objectid"].asString();
            CCLOG("LobbySceneLogic::processResponseForRegisterUser: Register successed, Email:%s Password:%s ObjectId:%s",email.c_str(),password.c_str(),objectId.c_str());
            
            // write to user default
            UserProfile* user = UserProfile::sharedUserProfile();
            user->setEmail(email);
            user->setObjectId(objectId);
            user->setPassword(password);
            
            
            setCurrentState(kLobbyStateLoginUser);
            loginUser();
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
            logined_ = true;
            
            // get object id 
            std::string objectId = response["objectid"].asString();
            UserProfile::sharedUserProfile()->setObjectId(objectId);
            
            setCurrentState(kLobbyStateIdle);
        }
    }
}


void LobbySceneLogic::processResponseForCreateRandomGame(bool curlState, const Json::Value &response)
{
    if(!curlState)
    {
        CCLOG("LobbySceneLogic::processResponseForCreateRandomGame:curl failed:%d",response["reason"].asInt());
//        setCurrentState(kLobbyStateRegisterUser);
    } 
    else 
    {
        bool result = response["result"].asBool();
        if(result == false)
        {
            CCLOG("LobbySceneLogic::processResponseForCreateRandomGame:create game failed:%d",response["reason"].asInt());
            setCurrentState(kLobbyStateIdle);
        }
        else 
        {
            
            int method = response["method"].asInt();
            if(method == kRandomGameCreate)
            {
                objectIdForCurrentGameId_ = response["gameid"].asString();
                setCurrentState(kLobbyStateWaitingForCreateRandomGame);
            }
            else 
                setCurrentState(kLobbyStateIdle);
            
            CCLOG("LobbySceneLogic::processResponseForCreateRandomGame:create game successed: id:%s method: %d",objectIdForCurrentGameId_.c_str(),method);
        }
    }
}

void LobbySceneLogic::processResponseForQueryCurrentRandomGame(bool curlState, const Json::Value &response)
{
    if(!curlState)
    {
        CCLOG("LobbySceneLogic::processResponseForQueryCurrentRandomGame:curl failed:%d",response["reason"].asInt());
    } 
    else 
    {
        bool result = response["result"].asBool();
        if(result == false)
        {
            CCLOG("LobbySceneLogic::processResponseForQueryCurrentRandomGame:query game failed:%d",response["reason"].asInt());
            setCurrentState(kLobbyStateIdle);
        }
        else 
        {
            const Json::Value& game = response["game"];
            if(game["state"].asInt() != 0)
            {
                CCLOG("LobbySceneLogic::processResponseForQueryCurrentRandomGame:query game successed:");
                setCurrentState(kLobbyStateIdle);
            }
        }
    }
}

void LobbySceneLogic::processResponseForQueryGamesForUser(bool curlState, const Json::Value &response)
{
    if(!curlState) {
        CCLOG("LobbySceneLogic::processResponseForQueryGamesForUser:curl failed:%d",response["reason"].asInt());
    } else {
        bool result = response["result"].asBool();
        if(result == false) {
            CCLOG("LobbySceneLogic::processResponseForQueryGamesForUser:query game failed:%d",response["reason"].asInt());
        } else {
            const Json::Value& games = response["games"];
            CCLOG("LobbySceneLogic::processResponseForQueryGamesForUser:query game successed:%d games found",games.size());
            
            UserProfile::sharedUserProfile()->synchronizeGameList(games);
        }
    }
    
    scene_->logicEvent(kLogicEventQueryGameForUserFinished);
}


void LobbySceneLogic::addReqeust(SimpleHttpRequest *request)
{
    pthread_mutex_lock(&requestMutex_);
    runningRequests_.push_back(request);
    pthread_mutex_unlock(&requestMutex_);
}




