//
//  Brush.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_Brush_h
#define drawstory_Brush_h

#include "cocos2d.h"
#include "types.h"

namespace PaintingScene {
    
    class Brush {
    public:
        Brush();
        virtual ~Brush();
        
        bool init();
        
        const cocos2d::ccColor3B& color() const { return color_; }
        void setColor(const cocos2d::ccColor3B& color);
        
        BrushWidth width() const { return width_; }
        void setWidth(BrushWidth width);
        
        void setPosition(const cocos2d::CCPoint& pos);
        void visit();
    protected:
        void updateTexture();
        
    protected:
        cocos2d::CCRenderTexture* texture_;
        cocos2d::ccColor3B color_;
        BrushWidth width_;
    };
    
    
}

#endif
