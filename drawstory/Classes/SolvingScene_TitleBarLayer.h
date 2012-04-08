//
//  SolveScene_TitleBarLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_SolvingScene_TitleBarLayer_h
#define drawstory_SolvingScene_TitleBarLayer_h

#include "cocos2d.h"

namespace SolvingScene {
    
    class TitleBarLayer : public cocos2d::CCLayer {
    public:
        LAYER_NODE_FUNC(TitleBarLayer)
        
        virtual bool init();
        virtual void onEnter();
    };
    
}

#endif
