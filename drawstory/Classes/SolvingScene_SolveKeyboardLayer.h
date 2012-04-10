//
//  SolvingScene_SolveKeyboardLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_SolvingScene_SolveKeyboardLayer_h
#define drawstory_SolvingScene_SolveKeyboardLayer_h

#include "cocos2d.h"
#include "KeyboardLayer.h"

namespace SolvingScene {
    
    class SolveKeyboardLayer : public KeyboardLayer {
    public:
        LAYER_NODE_FUNC(SolveKeyboardLayer)
        
        virtual bool init();
    protected:
        
    };
    
}


#endif
