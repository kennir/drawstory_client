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
#include "LevelHelperLoader.h"

class RegisterLayer;
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
    
    LobbySceneLogic* logic() const { return logic_; }
#pragma mark virtual override
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    
protected:
    void onLogicChanged();
    

    void addRegisterLayer();
    void removeRegisterLayer();
protected:
    LevelHelperLoader* level_;
    LobbySceneLogic* logic_;
    
    // tracking node for touch
    LHSprite* trackingNode_;
    
    // popup layers
    RegisterLayer* registerLayer_;
};



#endif
