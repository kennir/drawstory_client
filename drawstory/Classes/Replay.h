//
//  Replay.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_Replay_h
#define drawstory_Replay_h

#include <string>

class Replay {
public:
    typedef struct _data{
        size_t originSize;
        std::string data;
        
        void set(const std::string& d,size_t s) { data = d; originSize = s; }
    } Data;
public:
    bool hasPaintReplay() const { return (!paintReplay_.data.empty()); }
    bool hasSolveReplay() const { return (!solveReplay_.data.empty()); }
    
    const Data& paintReplay() const { return paintReplay_; }
    const Data& solveReplay() const { return solveReplay_; }
    
    void setPaintReplay(const std::string& data,size_t originSize) {
        paintReplay_.set(data,originSize);
    }
    
    void setSolveReplay(const std::string& data,size_t originSize) { 
        solveReplay_.set(data,originSize);
    }
protected:
    Data paintReplay_;
    Data solveReplay_;
};

#endif
