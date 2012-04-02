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

typedef enum {
    kCommandTypeDraw,
    kCommandTypeErase,
    kCommandTypeReset
} CommandType;

class Command {
public:
    Command(CommandType ct) : type_(ct) { }
    CommandType type() const { return type_; }
    
    static long nowMillisecond() { 
        cocos2d::cc_timeval time;
        cocos2d::CCTime::gettimeofdayCocos2d(&time,NULL);
        return (time.tv_sec * 1000 + time.tv_usec / 1000);
    }
protected:
    CommandType type_;
};


class PointsCommand : public Command {
public:
    typedef struct _PN {
        cocos2d::CCPoint pt;
        long ms;
        _PN(const cocos2d::CCPoint& p,long m) : pt(p), ms(m) {  }
    } PointNode;
    
    PointsCommand(CommandType ct) : Command(ct) { }
    virtual ~PointsCommand() { }
    
    void push(const cocos2d::CCPoint& localPos) { 
        nodes_.push_back(PointNode(localPos,nowMillisecond()));
    }
    
protected:
    std::vector< PointNode > nodes_; 
    
};


class DrawCommand : public PointsCommand {
public:
    DrawCommand(BrushWidth bw,const cocos2d::ccColor3B& bc) 
    : PointsCommand(kCommandTypeDraw), brushWidth_(bw), brushColor_(bc) { }
    virtual ~DrawCommand() { }
protected:
    BrushWidth brushWidth_;
    cocos2d::ccColor3B brushColor_;
};


class EraseCommand : public PointsCommand {
public:
    EraseCommand() : PointsCommand(kCommandTypeErase) { }
    virtual ~EraseCommand() { }
};


class ResetCommand : public Command {
public:
    ResetCommand() : Command(kCommandTypeReset) { }
    virtual ~ResetCommand() { }
};

class CommandQueue {
public:
    typedef struct _CI{
        Command* cmd;
        long ms;    // ms for push
        _CI(Command* c) : cmd(c), ms(Command::nowMillisecond()) { }
    } CommandInfo;
    
    void beginCommand(Command* command) { 
        commands_.push(CommandInfo(command));
    }
    
    Command* current() { return commands_.back().cmd; }
    
protected:
    std::queue< CommandInfo > commands_;
};

#endif
