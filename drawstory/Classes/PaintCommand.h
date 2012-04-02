//
//  PaintCommand.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_PaintCommand_h
#define drawstory_PaintCommand_h

#include "cocos2d.h"
#include "platform.h"
#include "vector"
#include "queue"
#include "types.h"




class PaintCommand {
public:
    typedef struct _PN {
        cocos2d::CCPoint pt;
        long ms;
        
        _PN(const cocos2d::CCPoint& p,long m) : pt(p), ms(m) {  }
    } PointNode;
    
    PaintCommand(BrushWidth bw,const cocos2d::ccColor3B& bc) : brushWidth_(bw), brushColor_(bc) { 
    }
    
    void push(const cocos2d::CCPoint& localPos) { 
        nodes_.push_back(PointNode(localPos,nowMillisecond()));
    }
    
    long nowMillisecond() const { 
        cocos2d::cc_timeval time;
        cocos2d::CCTime::gettimeofdayCocos2d(&time,NULL);
        return (time.tv_sec * 1000 + time.tv_usec / 1000);
    }
protected:
    std::vector< PointNode > nodes_; 
    BrushWidth brushWidth_;
    cocos2d::ccColor3B brushColor_;
};


class CommandQueue {
public:
    void beginCommand(PaintCommand command) { 
        commands_.push(command);
    }
    
    PaintCommand& current() { return commands_.back(); }
    
protected:
    std::queue< PaintCommand > commands_;
};

#endif
