//
//  PaintCommand.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintCommand.h"
#include "zlib.h"
#include "ybase64.h"

void PointsCommand::serialize(Json::Value& value) const {
    Command::serialize(value);
    Json::Value& points = value["points"];
    
    long timeInMs = 0;
    long previousTimeInMs = 0;
    
    points.resize(nodes_.size());
    for(int index = 0; index < nodes_.size(); ++index) {
        Json::Value& point = points[index];
        point["pt"]["x"] = nodes_[index].pt.x;
        point["pt"]["y"] = nodes_[index].pt.y;
        
        timeInMs = (!index) ? 0 : (nodes_[index].ms - previousTimeInMs);
        
        CC_ASSERT(timeInMs >= 0);
        point["ms"] = (Json::UInt)timeInMs;
        previousTimeInMs = nodes_[index].ms;
    }
}

void DrawCommand::serialize(Json::Value &value) const {
    PointsCommand::serialize(value);
    // write brush width and color
    value["brushwidth"] = brushWidth_;
    value["brushColor"]["r"] = brushColor_.r;
    value["brushColor"]["g"] = brushColor_.g;
    value["brushColor"]["b"] = brushColor_.b;
}

std::string CommandQueue::serialize() const {
    Json::Value value = serializeToJson();
    Json::FastWriter writer;
    std::string output = writer.write(value);
    
    // compress it 
    uLong length = compressBound(output.size());
    std::vector<unsigned char> buffer(length);
    int compressRet = compress2(&buffer.front(), &length, (unsigned char*)output.c_str(), output.size(), Z_BEST_COMPRESSION);
    if(compressRet != Z_OK){
        CCLOG("Can't compress painting record");
        buffer.clear();
    } else {
        CCLOG("painting command queue compressed: origin size:%u, new size:%u",output.size(),length);
        buffer.resize(length);
    }
    
    // release output
    output.clear(); 

    // then compress to bese64
    size_t sizeofBase64 = 0;
    char* base64 = static_cast<char*>(ybase64_encode_alloc(&buffer.front(), buffer.size(), &sizeofBase64));
    
    // release buffer
    //buffer.clear();

    CCLOG("base64 size is:%u",sizeofBase64);
    std::string base64String(base64);
    // release base64 buffer
    free(base64);
    
//    // try uncompress back
//    size_t unbase64Size;
//    char* unbase64 = static_cast<char*>(ybase64_decode_alloc(base64String.c_str(), base64String.size(), &unbase64Size));
//    CCLOG("unbase64 size is:%u",unbase64Size);
//    
//    int issame = memcmp((void*)unbase64, &buffer.front(), buffer.size());
//    CCLOG("issame is:%d",issame);
//
//    // try uncompress
//    unsigned char* unzipBuffer = new unsigned char[1024*1024];
//    uLongf unzipLen = 1024*1024;
//    int uncompressRet = uncompress(unzipBuffer, &unzipLen, (unsigned char*)unbase64, unbase64Size);
//    CCLOG("unzip size is:%u ret is:%d",unzipLen,uncompressRet);
//    
//    free(unbase64);
//    delete[] unzipBuffer;
    
    return base64String;
}


Json::Value CommandQueue::serializeToJson() const {
    Json::Value value;
    
    long timeInMs = 0;
    long previousTimeInMs = 0;
    
    value.resize(commands_.size());
    for (int index = 0; index < commands_.size(); ++index) {
        Json::Value& ci = value[index];
        
        timeInMs = (!index) ? 0 : (commands_[index].ms - previousTimeInMs);
        
        CC_ASSERT(timeInMs >= 0);
        
        ci["ms"] = (Json::UInt)timeInMs;
        previousTimeInMs = commands_[index].ms;
        
        Json::Value& cmd = ci["cmd"];
        commands_[index].cmd->serialize(cmd);
    }
    
    return value;
}

void CommandQueue::clear() { 
    std::vector< CommandInfo >::iterator it = commands_.begin();
    while (it != commands_.end()) {
        delete (*it).cmd;
        it = commands_.erase(it);
    }
}
