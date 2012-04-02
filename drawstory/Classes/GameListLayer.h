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

class GameLabelLayer;
class GameListLayer : public cocos2d::CCLayer {
public:
    LAYER_NODE_FUNC(GameListLayer)
    
    virtual bool init();
    
    void synchronizeGameList();
    
protected:
    // remove all labels from layer and put them to pool 
    void removeAllLabels();
    void addLabel(const cocos2d::CCPoint& pos,GameLabelLayer* label);
    GameLabelLayer* createLabel();
protected:
    std::list<GameLabelLayer*> usedLabel_;
    std::stack<GameLabelLayer*> labelPool_;
};

#endif
