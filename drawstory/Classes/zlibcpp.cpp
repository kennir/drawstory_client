//
//  zlibcpp.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "zlibcpp.h"
#include "cocos2d.h"


std::vector<unsigned char> zlib_compress(const unsigned char* src,size_t srcSize) {
    size_t length = compressBound(srcSize);
    std::vector<unsigned char> dest(length);
    
    int ret = compress2(&dest.front(),&length,src,srcSize,Z_BEST_COMPRESSION);
    if(ret != Z_OK) {
        CCLOG("compress2 failed!");
        dest.clear();
    } else 
        dest.resize(length);
    
    return dest;
}


std::vector<unsigned char> zlib_decompress(const unsigned char* src,size_t srcSize,size_t unzippedSize) {
    std::vector<unsigned char> dest(unzippedSize);
    size_t size = unzippedSize;
    int ret = uncompress(&dest.front(), &size, src, srcSize);
    if(ret != Z_OK) {
        CCLOG("uncompress failed!");
        dest.clear();
    } 
    return dest;
}


