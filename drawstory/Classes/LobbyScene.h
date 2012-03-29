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



class LobbyScene : public cocos2d::CCLayer
{
public:
#pragma mark static functions
    static cocos2d::CCScene* scene();
    LAYER_NODE_FUNC(LobbyScene)
public:
    virtual bool init();
protected:
    LevelHelperLoader* level_;
};



#endif
