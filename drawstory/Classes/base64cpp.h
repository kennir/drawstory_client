//
//  base64cpp.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_base64cpp_h
#define drawstory_base64cpp_h

#include <string>
#include <vector>

std::string base64_encode(unsigned char const* , unsigned int len);
std::vector<unsigned char> base64_decode(std::string const& s);


#endif
