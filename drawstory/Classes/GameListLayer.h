//
//  GameListLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_GameListLayer_h
#define drawstory_GameListLayer_h

#include "cocos2d.h"
#include "list"
#include "stack"

class GameLabelNode;
class GameListLayer : public cocos2d::CCLayer{
    typedef enum { kTouchTargetNone = 0, kTouchTargetNode, kTouchTargetLayer } TouchTarget;
public:
    LAYER_NODE_FUNC(GameListLayer)
    
    virtual ~GameListLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event); 
    
    void synchronizeGameList();
protected:
    void removeAllNodes();
    GameLabelNode* createLabelNode();
protected:
    std::list<GameLabelNode*> nodes_;
    std::stack<GameLabelNode*> pool_;
};


#endif
