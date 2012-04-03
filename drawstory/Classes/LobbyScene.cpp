//
//  LobbyScene.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LobbyScene.h"
#include "LobbySceneLogic.h"
#include "RegisterLayer.h"
#include "MessageLayer.h"
#include "GameListLayer.h"
#include "PaintingLayer.h"
#include "TitleBarLayer.h"

using namespace cocos2d;

// height of lobby scene
static const float kLobbySceneHeight = 200.0f;


// tags
enum { 
    kTagRegisterLayer = 1000,
    kTagTitleBarLayer,
    kTagGameListLayer,
    kTagMessageLayer,
    kTagPaintingLayer, 
};

// z-values
enum { 
    kZUILabels          = 1,
    kZTitleBarLayer     = 2,
    kZGameListLayer     = 2,
    kZRegisterLayer     = 3,
    kZPaintingLayer     = 3,
    kZMessageLayer      = 3, 
};

enum { kMsgTagWaitingQueryRandomGame = 1, };

CCScene* LobbyScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = LobbyScene::node();
    scene->addChild(layer);
    
    return scene;
}


LobbyScene::LobbyScene()
: logic_(NULL)
, registerLayer_(NULL)
, messageLayer_(NULL)
, paintingLayer_(NULL)
{
    pthread_mutex_init(&stateMutex_, NULL);
    pthread_mutex_init(&eventMutex_, NULL);
}

LobbyScene::~LobbyScene()
{
    pthread_mutex_destroy(&stateMutex_);
    pthread_mutex_destroy(&eventMutex_);
    
    CC_SAFE_RELEASE_NULL(registerLayer_);
    CC_SAFE_RELEASE_NULL(messageLayer_);
    CC_SAFE_RELEASE_NULL(gameListLayer_);
    CC_SAFE_RELEASE_NULL(paintingLayer_);
    
    delete logic_;
}

bool LobbyScene::init()
{
    bool result = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());
        // initialize levels
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // Create logic system
        logic_ = new LobbySceneLogic(this);
        CC_BREAK_IF(!logic_->init());
        
        
        //== background
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("background");
        CC_BREAK_IF(!background);
        background->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height * 0.5f));
        addChild(background);
        
        
        TitleBarLayer* titlebar = TitleBarLayer::node();
        CC_BREAK_IF(!titlebar);
        titlebar->setPosition(CCPointMake(0, 422.0f));
        addChild(titlebar,kZTitleBarLayer,kTagTitleBarLayer);
        
        // add game list node
        gameListLayer_ = GameListLayer::node();
        gameListLayer_->retain();
        gameListLayer_->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height - 100.0f));
        gameListLayer_->setAnchorPoint(CCPointMake(0.5f,1.0f));      // center left conron
        addChild(gameListLayer_,kZGameListLayer);
        

        
        
        result = true;
    } while (0);
    return result;
}



void LobbyScene::logicChanged()
{
    pthread_mutex_lock(&stateMutex_);
    stateQueue_.push(std::make_pair(static_cast<int>(logic_->previousState()), static_cast<int>(logic_->currentState())));
    pthread_mutex_unlock(&stateMutex_);

    schedule(schedule_selector(LobbyScene::processLogicChanged));
}



void LobbyScene::processLogicChanged()
{
    unschedule(schedule_selector(LobbyScene::processLogicChanged));
    
    pthread_mutex_lock(&stateMutex_);
    
    
    while(!stateQueue_.empty())
    {
        std::pair<int,int> states = stateQueue_.front();
        stateQueue_.pop();
        
        switch (states.first) {
            case kLobbyStateWaitingForRegisterUser:
                removeRegisterLayer();
                break;
            case kLobbyStateWaitingForCreateRandomGame:
                removeMessageLayer();
                CCLOG("==========unscheduled: queryCurrentRandomGame");
                unschedule(schedule_selector(LobbyScene::queryCurrentRandomGame));
                break;
            default:
                break;
        }
        
        switch (states.second) {
            case kLobbyStateRegisterUser:
                addRegisterLayer();
                break;
            case kLobbyStateWaitingForRegisterUser:
                break;
            case kLobbyStateLoginUser:
                break;
            case kLobbyStateWaitingForCreateRandomGame:
                addMessageLayer("等待匹配玩家...",
                                true,       // show cancel button
                                this,       // delegate
                                callfunc_selector(LobbyScene::onMessageLayerForQueryRandomGameCancelled),
                                kMsgTagWaitingQueryRandomGame);
                // schedule 
                CCLOG("==========scheduled: queryCurrentRandomGame");
                schedule(schedule_selector(LobbyScene::queryCurrentRandomGame),5.0f);
                break;
            case kLobbyStateIdle:
                refreshGamesForUser();
                break;
            default:
                break;
        }
    }
    pthread_mutex_unlock(&stateMutex_);
}

void LobbyScene::logicEvent(LogicEvent event) {
    pthread_mutex_lock(&eventMutex_);
    eventQueue_.push(event);
    pthread_mutex_unlock(&eventMutex_);
    
    schedule(schedule_selector(LobbyScene::processLogicEvent));
}

void LobbyScene::processLogicEvent() {
    unschedule(schedule_selector(LobbyScene::processLogicEvent));
    
    pthread_mutex_lock(&eventMutex_);
    
    while(!eventQueue_.empty()){
        LogicEvent e = eventQueue_.front();
        eventQueue_.pop();
        
        switch (e) {
            case kLogicEventQueryGameForUserFinished:
                CCLOG("==========scheduled: refreshGamesForUser");
                schedule(schedule_selector(LobbyScene::refreshGamesForUser),30.0f);
                // update game list
                gameListLayer_->synchronizeGameList();
                break;
            default:
                break;
        }
    }
    
    pthread_mutex_unlock(&eventMutex_);
}

void LobbyScene::queryCurrentRandomGame()
{
    logic_->queryCurrentRandomGame();
}

void LobbyScene::onMessageLayerForQueryRandomGameCancelled()
{
    logic_->cancelQueryCurrentRandomGame();
}

void LobbyScene::refreshGamesForUser()
{
    CCLOG("==========unschedule: refreshGamesForUser");
    unschedule(schedule_selector(LobbyScene::refreshGamesForUser));
    logic_->queryGamesForUser();
}


void LobbyScene::addRegisterLayer()
{
    if(!registerLayer_)
    {
        registerLayer_ = RegisterLayer::node();
        registerLayer_->retain();
        addChild(registerLayer_,kZRegisterLayer,kTagRegisterLayer);
    }
    else 
    {
        if(!getChildByTag(kTagRegisterLayer))
            addChild(registerLayer_,kZRegisterLayer,kTagRegisterLayer);     
    }
}

void LobbyScene::removeRegisterLayer()
{
    removeChildByTag(kTagRegisterLayer, false);
}

void LobbyScene::addMessageLayer(const std::string& message,
                                 bool showCancelButton,
                                 cocos2d::CCObject* delegate,
                                 cocos2d::SEL_CallFunc selector,
                                 int msgTag)
{
    if(!messageLayer_)
    {
        messageLayer_ = MessageLayer::layerWithLabel(message,showCancelButton,delegate,selector,msgTag);
        messageLayer_->retain();
        addChild(messageLayer_,kZMessageLayer,kTagMessageLayer);
    }
    else 
    {
        if(!getChildByTag(kTagMessageLayer))
            addChild(messageLayer_,kZMessageLayer,kTagMessageLayer);
        messageLayer_->setLabel(message);
        messageLayer_->setCancelButton(showCancelButton,delegate,selector);
        messageLayer_->setMessageTag(msgTag);
    }
}


void LobbyScene::removeMessageLayer()
{
    removeChildByTag(kTagMessageLayer, false);
}


void LobbyScene::addPaintingLayer() {
    if(!paintingLayer_) {
        paintingLayer_ = PaintingScene::PaintingLayer::node();
        paintingLayer_->retain();
        addChild(paintingLayer_,kZPaintingLayer,kTagPaintingLayer);
    } else {
        if(!getChildByTag(kTagPaintingLayer))
            addChild(paintingLayer_,kZPaintingLayer,kTagPaintingLayer);
    }
}

void LobbyScene::removePaintingLayer() {
    removeChildByTag(kTagPaintingLayer, false);
}

