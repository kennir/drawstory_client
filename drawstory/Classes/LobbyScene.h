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
#pragma mark virtual override
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    
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

    void onMessageLayerForQueryRandomGameCancelled();
    
    void queryCurrentRandomGame();
    void refreshGamesForUser();
    
    cocos2d::CCSprite* hitTestWithButton(const cocos2d::CCPoint& localPos);
protected:
    LobbySceneLogic* logic_;
    
    // tracking node for touch
    cocos2d::CCSprite* trackingNode_;
    
    
    // popup layers
    RegisterLayer* registerLayer_;
    MessageLayer* messageLayer_;
    
    pthread_mutex_t stateMutex_;
    std::queue<std::pair<int,int> > stateQueue_;
    
    pthread_mutex_t eventMutex_;
    std::queue<LogicEvent> eventQueue_;
};



#endif
