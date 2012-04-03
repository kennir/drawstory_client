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
    LAYER_NODE_FUNC(CanvasLayer)
    CanvasLayer();
    virtual ~CanvasLayer();
    
    Brush* brush() const { return brush_; }
    Brush* eraser() const { return eraser_; }
    
    PaintMode paintMode() const { return paintMode_; }
    
    void reset();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);    
protected:
    Brush* brush_;
    Brush* eraser_;
    
    PaintMode paintMode_;

    CommandQueue commandQueue_;
    
    bool drawing_;
    
    cocos2d::CCPoint previousLocalPosition_;
    cocos2d::CCRenderTexture* target_;
    cocos2d::CCRect layerRect_;
};

#endif
