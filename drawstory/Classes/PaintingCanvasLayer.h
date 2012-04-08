//
//  PaintingCanvasLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PaintingCanvasLayer_h
#define drawstory_PaintingCanvasLayer_h

#include "cocos2d.h"
#include "CanvasLayer.h"

namespace PaintingScene {
    
    class PaintingCanvasLayer : public CanvasLayer {
    public:
        LAYER_NODE_FUNC(PaintingCanvasLayer)
        
        PaintingCanvasLayer();
        virtual ~PaintingCanvasLayer();
        
        virtual bool init();
        virtual void resetToInitial();
        
        virtual void onEnter();
        virtual void onExit();
        
        virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event); 
        
        PaintingScene::Brush* brush() const { return brush_; }
        PaintingScene::Brush* eraser() const { return eraser_; }
        
        PaintMode paintMode() const { return paintMode_; }
        void setPaintMode(PaintMode newMode) { paintMode_ = newMode; }
    protected:
        PaintMode paintMode_;
        PaintingScene::Brush* brush_;
        PaintingScene::Brush* eraser_;
        
        bool drawing_;
        cocos2d::CCPoint previousLocalPosition_;
    };
    
    
}


#endif
