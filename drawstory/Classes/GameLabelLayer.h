//
//  GameLabelLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_GameLabelLayer_h
#define drawstory_GameLabelLayer_h

#include "cocos2d.h"
#include "LevelHelperLoader.h"

class GameLabelLayer : public cocos2d::CCLayer{
public:
    LAYER_NODE_FUNC(GameLabelLayer)
public:    
    GameLabelLayer();
    virtual ~GameLabelLayer();
    virtual bool init();
    
    void setGameId(const std::string& gameId) { gameId_ = gameId; }
    const std::string& gameId() const { return gameId_; }
protected:
    LevelHelperLoader* level_;
    std::string gameId_;
    
    cocos2d::CCLabelTTF* turnLabel_;
};


#endif
