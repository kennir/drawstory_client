//
//  SolvingScene_SolveKeyboardLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SolvingScene_SolveKeyboardLayer.h"

using namespace cocos2d;

namespace SolvingScene {
    
    bool SolveKeyboardLayer::init() {
        bool result = false;
        do {
            CC_BREAK_IF(!KeyboardLayer::init());
            
            
            result = true;
        } while (0);
        return result;
    }
    
}
