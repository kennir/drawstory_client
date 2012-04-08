//
//  CanvasLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_CanvasLayer_h
#define drawstory_CanvasLayer_h

#include "cocos2d.h"
#include "Brush.h"
#include "PaintCommand.h"




class CanvasLayer : public cocos2d::CCLayer {
public:
    CanvasLayer();
    virtual ~CanvasLayer();
    
    const PaintCommandQueue& commandQueue() const { return commandQueue_; }
    
    virtual bool init();
    virtual void resetToInitial();
    void deletePainting();
protected:

    PaintCommandQueue commandQueue_;
    

    cocos2d::CCRenderTexture* target_;
    cocos2d::CCRect layerRect_;
};


    
#endif
