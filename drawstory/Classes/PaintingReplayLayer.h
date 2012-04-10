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
    virtual ~PaintingReplayLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void update(cocos2d::ccTime dt);
    
    
protected:    
    void processCommand(const Command* command);
    void processCommandTouchMoved(const TouchCommand* command);
    void processCommandTouchEnded(const TouchCommand* command);
    
protected:
    long timePassed_;
    
    PaintMode paintMode_;
    Brush* brush_;
    Brush* eraser_;
    
    cocos2d::CCPoint previousPosition_;
    
    std::vector< Command* >::const_iterator currentPos_;
};

    


#endif
