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
    kHttpRequestSendPainting,
    kHttpRequestQueryReplay,
};

enum {
    kRandomGameCreate   = 1,
    kRandomGameJoin,
};

static const char* _hostURL = "http://5.44.94.85:3000/";

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
        case kHttpRequestSendPainting:
            processResponseForSendPainting(result,response);
            break;
        case kHttpRequestQueryReplay:
            processResponseForQueryReplay(result, response);
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
    std::ostringstream url;
    url << _hostURL << "register";
    
    std::ostringstream body;
    body << "email=" << email << "&username=" << username;
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodPost, 
                                                                             this,
                                                                             kHttpRequestRegister);
    request->setPostText(body.str());
    request->retain();
    addReqeust(request);
    request->start();

    setCurrentState(kLobbyStateWaitingForRegisterUser);
}


void LobbySceneLogic::loginUser()
{
    
    UserProfile* user = UserProfile::sharedUserProfile();  
    // make URL
    std::ostringstream url;
    url << _hostURL << "login";
    
    std::ostringstream body;
    body << "email=" << user->email() << "&password=" << user->password();
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodPost, 
                                                                             this,
                                                                             kHttpRequestLogin);
    request->setPostText(body.str());
    request->retain();
    addReqeust(request);
    request->start();

    setCurrentState(kLobbyStateWaitingForLoginUser);
}


void LobbySceneLogic::createRandomGame()
{
    if(logined_) {
        UserProfile* user = UserProfile::sharedUserProfile();
        std::ostringstream url;
        url << _hostURL << "game/random";
        
        std::ostringstream body;
        body << "email=" << user->email();
        
        SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                                 SimpleHttpRequest::kHttpMethodPost, 
                                                                                 this,
                                                                                 kHttpRequestCreateRandomGame);
        request->setPostText(body.str());
        request->retain();
        addReqeust(request);
        request->start();
        
        setCurrentState(kLobbyStateWaitingForCreateRandomGame);
    }
}

void LobbySceneLogic::cancelCreateRandomGame() {
    cancelAllRequestsForTag(kHttpRequestQueryCurrentRandomGame);
    setCurrentState(kLobbyStateIdle);
}

void LobbySceneLogic::queryCurrentRandomGame()
{
//    CC_ASSERT(!objectIdForCurrentGameId_.empty());
    
//    UserProfile* user = UserProfile::sharedUserProfile();
    std::ostringstream url;
    url << _hostURL << "game/" << GlobalData::sharedGlobalData()->currentGameId();
        
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodGet, 
                                                                             this,
                                                                             kHttpRequestQueryCurrentRandomGame);
    
    request->retain();
    addReqeust(request);
    request->start();
}

void LobbySceneLogic::cancelQueryCurrentRandomGame() {
    cancelAllRequestsForTag(kHttpRequestQueryCurrentRandomGame);
    
    // notify server to remove game if not paired
//    UserProfile* user = UserProfile::sharedUserProfile();
    std::ostringstream url;
    url << _hostURL << "game/" << GlobalData::sharedGlobalData()->currentGameId();
    
    std::ostringstream body;
    body << "owner=" << UserProfile::sharedUserProfile()->objectId();
    
    
    SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                             SimpleHttpRequest::kHttpMethodDelete, 
                                                                             this,
                                                                             kHttpRequestCancelQueryCurrentRandomGame);

    
    request->setPostText(body.str());
    request->retain();
    addReqeust(request);
    request->start();
    
    // set to idle state
    setCurrentState(kLobbyStateIdle);
}


void LobbySceneLogic::queryGamesForUser()
{
    // 只有当前没有请求的时候才会查询游戏列表,放在LobbyScene里判断
//    if(runningRequests_.empty()) {
        UserProfile* user = UserProfile::sharedUserProfile();
        CC_ASSERT(!user->objectId().empty());
        
        std::ostringstream url;
        url << _hostURL << "user/games/" << user->objectId();
        
        SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                                 SimpleHttpRequest::kHttpMethodGet, 
                                                                                 this,
                                                                                 kHttpRequestQueryGamesForUser);
        
        
        request->retain();
        addReqeust(request);
        request->start();
//    }
}

void LobbySceneLogic::cancelAllRequestsForTag(int tag) {
    pthread_mutex_lock(&requestMutex_);
    
    std::list<SimpleHttpRequest*>::iterator it = runningRequests_.begin();
    while( it != runningRequests_.end())
    {
        SimpleHttpRequest* req = (*it);
        if(req->tag() == tag)
        {
            req->cancel();
            req->release();
            it = runningRequests_.erase(it);
        } else 
            ++it;
    }
    
    pthread_mutex_unlock(&requestMutex_);
}

void LobbySceneLogic::playGame(const std::string &gameId) {
    Game* game = UserProfile::sharedUserProfile()->findGame(gameId);
    if(!game) {
        CCLOG("Can't find game for id:%s",gameId.c_str());
    } else {
        
        GlobalData* data = GlobalData::sharedGlobalData();
        data->setCurrentGameId(gameId);
        // temporary
        data->setCurrentDifficult(kDifficultEasy);
        
        
        if(game->question().replayId().empty()){
            // goto paint scene
            setCurrentState(kLobbyStatePaintQuestion);
        } else {
            queryReplay();
        }
    }
}

void LobbySceneLogic::sendPainting(const std::string &painting,size_t originSize) {
    GlobalData* gd = GlobalData::sharedGlobalData();
    CC_ASSERT(!gd->currentGameId().empty());
    
    Game* game = UserProfile::sharedUserProfile()->findGame(gd->currentGameId());
    if(!game) {
        CCLOG("LobbySceneLogic::sendPainting: Can't find game,already removed?");
    } else {
        // write to game
        CC_ASSERT(game->isMyTurn());
        game->commitTurn(gd->currentDifficult(), painting, originSize, "", 0);
        
        // send request for all games
        sendPaintingRequestForAllGames();
    }
    
    // switch to idle
    setCurrentState(kLobbyStateIdle); 
}


void LobbySceneLogic::sendPaintingRequestForAllGames() {    
    UserProfile* user = UserProfile::sharedUserProfile();
    
    std::ostringstream url;
    std::ostringstream body;
    std::list<Game*>::const_iterator it = user->games().begin();
    for(; it != user->games().end(); ++it){
        Game* game = (*it);
        
        if(game->replaySendState() == kSendStateWaiting) {
            url.str("");
            url << _hostURL << "painting/" << user->objectId() << "/" << game->objectId() << "/" << game->question().difficult();
            
            const Replay& replay = game->replay();
            
            body.str("");
            body << "paintingsize=" << replay.paintReplay().originSize << "&painting=" << replay.paintReplay().data 
                << "&solvingsize=" << replay.solveReplay().originSize << "&solving=" << replay.solveReplay().data;
            
            
            SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                                     SimpleHttpRequest::kHttpMethodPost, 
                                                                                     this,
                                                                                     kHttpRequestSendPainting);
            
            
            game->beginSendReplay();
            request->setPostText(body.str());
            request->retain();
            addReqeust(request);
            request->start();
        }
    }
    
    // notify state changed
    scene_->logicEvent(kLogicEventGameStateChanged);
}


void LobbySceneLogic::queryReplay() {
    
    Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
    if(game) {
        CC_ASSERT(!game->question().replayId().empty());
        
        std::ostringstream url;
        url << _hostURL << "replay/" << game->question().replayId();
        
        SimpleHttpRequest* request = SimpleHttpRequest::simpleHttpRequestWithURL(url.str(), 
                                                                                 SimpleHttpRequest::kHttpMethodGet, 
                                                                                 this,
                                                                                 kHttpRequestQueryReplay); 
        request->retain();
        addReqeust(request);
        request->start();
        
        setCurrentState(kLobbyStateWaitingForQueryReplay);      
    } else {
        CCLOG("WARNNING:LobbySceneLogic::queryReplay() can't find game, already removed?");
    }
    
}

void LobbySceneLogic::cancelQueryReplay() {
    cancelAllRequestsForTag(kHttpRequestQueryReplay);
    setCurrentState(kLobbyStateIdle);
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
        setCurrentState(kLobbyStateIdle);
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
                GlobalData::sharedGlobalData()->setCurrentGameId( response["gameid"].asString() );
                setCurrentState(kLobbyStateWaitingForQueryCurrentRandomGame);
            }
            else 
                setCurrentState(kLobbyStateIdle);
            
            CCLOG("LobbySceneLogic::processResponseForCreateRandomGame:create game successed: id:%s method: %d",GlobalData::sharedGlobalData()->currentGameId().c_str(),method);
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

void LobbySceneLogic::processResponseForSendPainting(bool curlState, const Json::Value &response) {
    if(!curlState) {
        CCLOG("LobbySceneLogic::processResponseForSendPainting:curl failed:%d",response["reason"].asInt());
    } else {
        bool result = response["result"].asBool();
        std::string gameId = response["gameid"].asString();
        Game* game = UserProfile::sharedUserProfile()->findGame(gameId);
        if(game){
            if(result == false) {
                CCLOG("LobbySceneLogic::processResponseForSendPainting:send painting failed:%d",response["reason"].asInt());
                game->endSendReplay(false);
            } else {
                CCLOG("LobbySceneLogic::processResponseForSendPainting:send painting successed:%d",response["reason"].asInt());
                game->endSendReplay(true);
            }
        }
        
        scene_->logicEvent(kLogicEventGameStateChanged);
    }
}

void LobbySceneLogic::processResponseForQueryReplay(bool curlState,const Json::Value& response) {
    if(!curlState) {
        CCLOG("LobbySceneLogic::processResponseQueryReplay:curl failed:%d",response["reason"].asInt());
        setCurrentState(kLobbyStateIdle); //
    } else {
        bool result = response["result"].asBool();
        if(!result) {
            CCLOG("LobbySceneLogic::processResponseQueryReplay:query replay failed:%d",response["reason"].asInt());
            setCurrentState(kLobbyStateIdle); //
        } else {
            // put to the game 
            Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
            if(!game) {
                CCLOG("LobbySceneLogic::processResponseQueryReplay:query replay failed,Cant find game:%d",response["reason"].asInt());
                setCurrentState(kLobbyStateIdle);
            } else {
                CCLOG("LobbySceneLogic::processResponseQueryReplay:query replay successed:%d",response["reason"].asInt());
                const Json::Value& replayJson = response["replay"];
                game->receiveReplay(replayJson["paintreplay"].asString(),
                                    replayJson["paintreplayoriginsize"].asUInt(),
                                    replayJson["solvereplay"].asString(), 
                                    replayJson["solvereplayoriginsize"].asUInt());
                // switch to next scene
                if(game->replay().hasSolveReplay()) {
                    setCurrentState(kLobbyStateReplayResult);
                } else if(game->replay().hasPaintReplay()) {
                    setCurrentState(kLobbyStateSolveQuestion);
                } else {
                    setCurrentState(kLobbyStateIdle);
                }
            }
            
        }
    }
}


void LobbySceneLogic::addReqeust(SimpleHttpRequest *request)
{
    pthread_mutex_lock(&requestMutex_);
    runningRequests_.push_back(request);
    pthread_mutex_unlock(&requestMutex_);
}




