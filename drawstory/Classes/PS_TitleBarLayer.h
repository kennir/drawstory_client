//
//  PS_TitleBarLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PS_TitleBarLayer_h
#define drawstory_PS_TitleBarLayer_h

#include "cocos2d.h"




namespace PaintingScene {
    class PaintingCanvasLayer;  
    class TitleBarLayer : public cocos2d::CCLayer {
    public:
        LAYER_NODE_FUNC(TitleBarLayer)
        
        TitleBarLayer();
        
        void setCanvasLayer(PaintingCanvasLayer* cl) { canvas_ = cl; }
        
        virtual bool init();
        virtual void onEnter();
        virtual void onExit();    
        
        virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);  
    protected:
        cocos2d::CCNode* hitTest(const cocos2d::CCPoint& localPos);
        void onNodeClicked(cocos2d::CCNode* button);
    protected:
        PaintingCanvasLayer* canvas_;
        cocos2d::CCNode* trackingNode_;
    };

}

#endif
