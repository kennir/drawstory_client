//
//  GlobalData.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GlobalData.h"

static GlobalData* _globalData = NULL;


GlobalData* GlobalData::sharedGlobalData() {
    if(!_globalData) {
        _globalData = new GlobalData;
    }
    return _globalData;
}

void GlobalData::purgeGlobalData() {
    delete _globalData;
    _globalData = NULL;
}





