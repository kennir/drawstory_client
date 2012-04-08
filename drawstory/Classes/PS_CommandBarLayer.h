//
//  PS_CommandBarLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PS_CommandBarLayer_h
#define drawstory_PS_CommandBarLayer_h

#include "cocos2d.h"
#include "types.h"


class LobbySceneLogic;
namespace PaintingScene {
    
    class Brush;
    class PaintingCanvasLayer;
    class CommandBarLayer : public cocos2d::CCLayer {
    public:
        LAYER_NODE_FUNC(CommandBarLayer)
        
        CommandBarLayer();
        
        void setCanvasLayer(PaintingCanvasLayer* canvas) { canvas_ = canvas; }
        
        virtual bool init();
        virtual void onEnter();
        virtual void onExit();
        
        
        virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);   
    protected:
        cocos2d::CCNode* hitTest(const cocos2d::CCPoint& localPos);
        
        // update visual state for button
        void beginTouchNode(cocos2d::CCNode* button);
        void endTouchNode(cocos2d::CCNode* button);
        void onNodeClicked(cocos2d::CCNode* button);
        
        // void slide up buttons
        void slideUpButtons(int beginTag,int endTag,const cocos2d::CCPoint& moveFrom);
        void slideDownButtons(int beginTag,int endTag,const cocos2d::CCPoint& moveTo);
        
        void prepareWidthMenuItems(Brush* brush,int beginTag,int endTag);
        void collapseBrushWidthMenu();
        void collapseEraserWidthMenu();
        
        cocos2d::CCNode* getNodeByWidth(BrushWidth width,int beginTag);
        LobbySceneLogic* getLogic();
        
    protected:
        cocos2d::CCNode* trackingNode_;
        bool brushWidthMenuExpanded_;
        bool eraserWidthMenuExpanded_;
        
        PaintingCanvasLayer* canvas_;
    };

}

#endif
