//
//  PaintingReplayLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PaintingReplayLayer_h
#define drawstory_PaintingReplayLayer_h

#include "cocos2d.h"
#include "CanvasLayer.h"
#include "PaintCommand.h"
#include "vector"
    
    
class PaintingReplayLayer : public CanvasLayer {
public:
    LAYER_NODE_FUNC(PaintingReplayLayer)
    
    PaintingReplayLayer();
    
    virtual bool init();
    virtual void update(cocos2d::ccTime dt);
protected:
    
    float timePassed_;
    
    std::vector<PaintCommandQueue::CommandInfo>::const_iterator currentIt_;
};

    


#endif
