//
//  zlibcpp.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_zlibcpp_h
#define drawstory_zlibcpp_h

#include "zlib.h"
#include "vector"

std::vector<unsigned char> zlib_compress(const unsigned char* src,size_t srcSize);
std::vector<unsigned char> zlib_decompress(const unsigned char* src,size_t srcSize,size_t unzippedSize);

#endif
