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

using namespace cocos2d;


enum { 
    kTagRegisterLayer = 1000,
    kTagGameListLayer,
    kTagMessageLayer,
    kTagPaintingLayer, 
    kTagStartGameButton 
};

enum { 
    kZUIButtons         = 1, 
    kZUILabels          = 1,
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
, trackingNode_(NULL)
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

        
        //== background
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("background");
        CC_BREAK_IF(!background);
        background->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height * 0.5f));
        addChild(background);
        
        //== top bar
        CCSprite* bannerup = CCSprite::spriteWithSpriteFrameName("banner_up");
        CC_BREAK_IF(!bannerup);
        bannerup->setPosition(CCPointMake(winSize.width * 0.5f, 
                                          winSize.height - (bannerup->getContentSize().height * 0.5f)));
        addChild(bannerup);
        
        //== start button
        CCSprite* startgame = CCSprite::spriteWithSpriteFrameName("startgame");
        CC_BREAK_IF(!startgame);
        startgame->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height - 100.0f));
        addChild(startgame,kZUIButtons,kTagStartGameButton);
        
        
        //== playing game
        CCSprite* gameplay = CCSprite::spriteWithSpriteFrameName("gameplay");
        CC_BREAK_IF(!gameplay);
        gameplay->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height - 180.0f));
        addChild(gameplay);
        

        // Create logic system
        logic_ = new LobbySceneLogic(this);
        CC_BREAK_IF(!logic_->init());
        
        
        // add game list node
        gameListLayer_ = GameListLayer::node();
        gameListLayer_->retain();
        gameListLayer_->setPosition(CCPointMake(winSize.width * 0.5f, winSize.height - 208.0f));
        gameListLayer_->setAnchorPoint(CCPointMake(0.5f,1.0f));      // top left conron
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

void LobbyScene::onEnter()
{
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,kTouchPriorityLobbyScene, true);
}

void LobbyScene::onExit()
{
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}


bool LobbyScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    trackingNode_ = hitTestWithButton(convertTouchToNodeSpace(touch));
    if(trackingNode_) {
        trackingNode_->setScale(0.9f);
    } else {
        // show painting layer for testing
        addPaintingLayer();
    }
    return true;
}

void LobbyScene::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(trackingNode_)
    {
        CCSprite* node = hitTestWithButton(convertTouchToNodeSpace(touch));
        if(node != trackingNode_)
        {
            trackingNode_->setScale(1.0f);
            trackingNode_ = NULL;
        }
    }
}

void LobbyScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    if(trackingNode_) {
        CCSprite* node = hitTestWithButton(convertTouchToNodeSpace(touch));
        if(node == trackingNode_) {
            // on button clicked
            logic_->createRandomGame();
        }         
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
}

void LobbyScene::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(trackingNode_)
    {
        trackingNode_->setScale(1.0f);
        trackingNode_ = NULL;
    }
    
}


CCSprite* LobbyScene::hitTestWithButton(const cocos2d::CCPoint &localPos)
{
    CCSprite* node = NULL;

    CCSprite* button = static_cast<CCSprite*>(getChildByTag(kTagStartGameButton));
    CC_ASSERT(button != NULL);
    if(CCRect::CCRectContainsPoint(button->boundingBox(), localPos))
        node = button;
    return node;
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
        paintingLayer_ = PaintingLayer::node();
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

