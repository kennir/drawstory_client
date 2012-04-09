//
//  PaintCommand.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "PaintCommand.h"
#include "zlib.h"
#include "base64cpp.h"
#include "zlibcpp.h"


size_t PaintCommandQueue::serialize(std::string& data) const {
    size_t result = 0;
    do {
        Json::FastWriter writer;
        // JSON
        std::string json = writer.write(serialize());
        // ZIP
        std::vector<unsigned char> zippedData = zlib_compress((const unsigned char*)json.c_str(),json.size());
        CC_BREAK_IF(zippedData.empty());
        // BASE64
        data = base64_encode(&zippedData.front(), zippedData.size());

        result = json.size();
    } while (0);
    return result;
}


Json::Value PaintCommandQueue::serialize() const {
    Json::Value value;

    if(!commands_.empty()) {
        long startMs = commands_.front()->millisecond();
        
        value.resize(commands_.size());
        for (int index = 0; index < commands_.size(); ++index) {
            commands_[index]->serialize(value[index], startMs);
        }
    }
    return value;
}


bool PaintCommandQueue::deserialize(const std::string &data, size_t originSize) {
    bool result = false;
    do {
        // UNBASE64
        std::vector<unsigned char> unbase64Data = base64_decode(data);
        CCLOG("zipped data length:%u",unbase64Data.size());
        // UNZIP
        std::vector<unsigned char> unzippedData = zlib_decompress(&unbase64Data.front(), unbase64Data.size(), originSize);
        CC_BREAK_IF(unzippedData.empty());
        
        
        Json::Value value;
        
        Json::Reader reader;
        bool ret = reader.parse((const char*)&unzippedData.front(), 
                                (const char*)&unzippedData.front() + unzippedData.size(),
                                value);

        CC_BREAK_IF(!ret);
        
        deserialize(value);
        
        result = true;
    } while (0);

    return result;
}

void PaintCommandQueue::clear() { 
    std::vector< Command* >::iterator it = commands_.begin();
    while (it != commands_.end()) {
        delete (*it);
        it = commands_.erase(it);
    }
}

void PaintCommandQueue::deserialize(const Json::Value &value) {
    for (int i = 0; i < value.size(); ++i) {
        const Json::Value& node = value[i];
        
        CommandType type = static_cast<CommandType>(node["type"].asInt());
        switch (type) {
            case kCommandTypeTouchBegan:
            case kCommandTypeTouchMoved:
            case kCommandTypeTouchEnded:
                push(new TouchCommand(node));
                break;
            case kCommandTypeReset:
                push(new ResetCommand(node));
                break;
            case kCommandTypeSetColor:
                push(new SetColorCommand(node));
                break;
            case kCommandTypeSetWidth:
                push(new SetWidthCommand(node));
                break;
            case kCommandTypeSetPaintMode:
                push(new SetPaintModeCommand(node));
                break;
            default:
                CCLOG("WARNNING:Unknown command type:%d",type);
                break;
        }
    }
}
