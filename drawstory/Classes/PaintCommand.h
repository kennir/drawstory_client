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
#include "json/json.h"


typedef enum {
    kCommandTypeTouchBegan = 1,
    kCommandTypeTouchMoved,
    kCommandTypeTouchEnded,
    kCommandTypeReset,
    kCommandTypeSetPaintMode,
    kCommandTypeSetWidth,
    kCommandTypeSetColor,
} CommandType;

class Command {
public:
    Command(CommandType ct) : type_(ct),ms_(nowMillisecond()) { }
    Command(const Json::Value& value)
    : type_(static_cast<CommandType>(value["type"].asInt()))
    , ms_(static_cast<long>(value["ms"].asUInt()))
    {  }
    
    CommandType type() const { return type_; }
    
    static long nowMillisecond() { 
        cocos2d::cc_timeval time;
        cocos2d::CCTime::gettimeofdayCocos2d(&time,NULL);
        return (time.tv_sec * 1000 + time.tv_usec / 1000);
    }
    
    virtual void serialize(Json::Value& value,long startMs) const {
        value["type"] = type_;
        value["ms"] = static_cast<Json::UInt>(ms_ - startMs);
    }
    
    long millisecond() const { return ms_; }
protected:
    CommandType type_;
    long ms_;
};




class TouchCommand : public Command {
public:
    TouchCommand(CommandType type,const cocos2d::CCPoint& touchedPoint)
    : Command(type), touchedPoint_(touchedPoint) {
        CC_ASSERT(type == kCommandTypeTouchBegan ||
                  type == kCommandTypeTouchMoved ||
                  type == kCommandTypeTouchEnded);
    }
    TouchCommand(const Json::Value& value) 
    : Command(value) {
        touchedPoint_.x = value["x"].asFloat();
        touchedPoint_.y = value["y"].asFloat();
    }
    
    virtual ~TouchCommand() { }
    
    virtual void serialize(Json::Value& value,long startMs) const {
        Command::serialize(value,startMs);
        value["x"] = touchedPoint_.x;
        value["y"] = touchedPoint_.y;
    }
    
protected:
    cocos2d::CCPoint touchedPoint_;
};



class ResetCommand : public Command {
public:
    ResetCommand(const Json::Value& value) : Command(value) { }
    ResetCommand() : Command(kCommandTypeReset) { }

    virtual ~ResetCommand() { }
};

class SetPaintModeCommand : public Command {
public:
    SetPaintModeCommand(const Json::Value& value) : Command(value) {
        mode_ = static_cast<PaintMode>(value["mode"].asInt());
    }
    
    SetPaintModeCommand(PaintMode mode) : Command(kCommandTypeSetPaintMode), mode_(mode) { }
    virtual ~SetPaintModeCommand() { }
    
    virtual void serialize(Json::Value& value,long startMs) const {
        Command::serialize(value,startMs);
        value["mode"] = mode_;
    }
protected:
    PaintMode mode_;
};

class SetWidthCommand : public Command {
public:
    SetWidthCommand(const Json::Value& value) : Command(value) {
        width_ = static_cast<BrushWidth>(value["width"].asInt());
    }
    
    SetWidthCommand(BrushWidth width) : Command(kCommandTypeSetWidth), width_(width) { }
    virtual ~SetWidthCommand() { }
    
    virtual void serialize(Json::Value& value,long startMs) const {
        Command::serialize(value,startMs);
        value["width"] = width_;
    }
protected:
    BrushWidth width_;
};


class SetColorCommand : public Command {
public:
    SetColorCommand(const Json::Value& value) : Command(value) {
        color_ = cocos2d::ccc3(value["r"].asInt(), value["g"].asInt(), value["b"].asInt());
    }
    
    SetColorCommand(const cocos2d::ccColor3B& color) : Command(kCommandTypeSetColor), color_(color) { }
    virtual ~SetColorCommand() { }
    
    virtual void serialize(Json::Value& value,long startMs) const {
        Command::serialize(value,startMs);
        value["r"] = color_.r;
        value["g"] = color_.g;
        value["b"] = color_.b;
    }
protected:
    cocos2d::ccColor3B color_;
};



class PaintCommandQueue {
public:
    virtual ~PaintCommandQueue() {
        this->clear();
    }
    
    void push(Command* cmd) { commands_.push_back(cmd); }
    
    bool empty() const { return commands_.empty(); }
    void clear();
    
    const std::vector<Command*>& commands() const { return commands_; }
    
    // Return size of origin data
    // data: zipped and base64ed string
    size_t serialize(std::string& data) const;
    
    bool deserialize(const std::string& data,size_t originSize);
protected:
    Json::Value serialize() const;
    void deserialize(const Json::Value& value);
    
protected:
    std::vector< Command* > commands_;
};

#endif
