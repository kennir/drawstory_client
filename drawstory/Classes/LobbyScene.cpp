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

using namespace cocos2d;


enum { kTagRegisterLayer = 1000 };

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
: level_(NULL)
, logic_(NULL)
, trackingNode_(NULL)
, registerLayer_(NULL)
{
    
}

LobbyScene::~LobbyScene()
{
    CC_SAFE_RELEASE_NULL(registerLayer_);
    
    delete logic_;
    delete level_;
}

bool LobbyScene::init()
{
    bool result = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());
        // load level's
        level_ = new LevelHelperLoader("lobbyscene.plhs");
        CC_BREAK_IF(!level_);
        level_->addObjectsToWorld(NULL, this);
        
        logic_ = new LobbySceneLogic(this);
        CC_BREAK_IF(!logic_->init());
        
        

        
        result = true;
    } while (0);
    return result;
}



void LobbyScene::logicChanged()
{
    schedule(schedule_selector(LobbyScene::onLogicChanged));
}

void LobbyScene::onLogicChanged()
{
    unschedule(schedule_selector(LobbyScene::onLogicChanged));
    
    // check state
    switch (logic_->currentState()) {
        case kLobbyStateRegisterUser:
            addRegisterLayer();
            break;
        case kLobbyStateWaitingForRegisterUser:
            removeRegisterLayer();
            break;
        default:
            break;
    }
    
}


void LobbyScene::addRegisterLayer()
{
    if(!registerLayer_)
    {
        registerLayer_ = RegisterLayer::node();
        addChild(registerLayer_,1,kTagRegisterLayer);
    }
    else 
    {
        if(!getChildByTag(kTagRegisterLayer))
            addChild(registerLayer_,1,kTagRegisterLayer);     
    }
}

void LobbyScene::removeRegisterLayer()
{
    removeChildByTag(kTagRegisterLayer, false);
}


void LobbyScene::onEnter()
{
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

void LobbyScene::onExit()
{
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}


bool LobbyScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
  
    return true;
}

void LobbyScene::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    
}

void LobbyScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    
}

void LobbyScene::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    
}



