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
public:
    LAYER_NODE_FUNC(GameListLayer)
    
    virtual ~GameListLayer();
    virtual bool init();
    
    void synchronizeGameList();
protected:
    void removeAllNodes();
    GameLabelNode* createLabelNode();
protected:
    std::list<GameLabelNode*> nodes_;
    std::stack<GameLabelNode*> pool_;
};


#endif
