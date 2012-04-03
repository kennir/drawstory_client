//
//  LobbyScene.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_LobbyScene_h
#define drawstory_LobbyScene_h

#include "cocos2d.h"
#include "types.h"

#include <queue>

namespace PaintingScene { class PaintingLayer; }



class GameListLayer;
class RegisterLayer;
class MessageLayer;
class LobbySceneLogic;
class LobbyScene : public cocos2d::CCLayer
{    
    
public:
#pragma mark static functions
    static cocos2d::CCScene* scene();
    LAYER_NODE_FUNC(LobbyScene)
public:
    LobbyScene();
    virtual ~LobbyScene();
    
    virtual bool init();
    
    // shoud call onLogicChanged on next tick
    void logicChanged();
    void logicEvent(LogicEvent event);
    
    LobbySceneLogic* logic() const { return logic_; }    
protected:
    void processLogicChanged();
    void processLogicEvent();
    
    void addRegisterLayer();
    void removeRegisterLayer();    
    void addMessageLayer(const std::string& message,
                         bool showCancelButton,
                         cocos2d::CCObject* delegate,
                         cocos2d::SEL_CallFunc selector,
                         int msgTag);
    void removeMessageLayer();
    
    void addPaintingLayer();
    void removePaintingLayer();

    void onMessageLayerForQueryRandomGameCancelled();
    
    void queryCurrentRandomGame();
    void refreshGamesForUser();
    
protected:
    LobbySceneLogic* logic_;

    
    // popup layers
    RegisterLayer* registerLayer_;
    MessageLayer* messageLayer_;
    PaintingScene::PaintingLayer* paintingLayer_;
    
    // game list node
    GameListLayer* gameListLayer_;
    
    pthread_mutex_t stateMutex_;
    std::queue<std::pair<int,int> > stateQueue_;
    
    pthread_mutex_t eventMutex_;
    std::queue<LogicEvent> eventQueue_;
};



#endif
