//
//  GameLabelNode.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_GameLabelNode_h
#define drawstory_GameLabelNode_h

#include "cocos2d.h"

class Game;
class GameLabelNode : public cocos2d::CCNode{
public:
    static cocos2d::CCNode* node();
public:
    virtual bool init();
    
    void setGame(const Game* game);
protected:
    std::string gameId_;

};


#endif
