//
//  GlobalData.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_GlobalData_h
#define drawstory_GlobalData_h

#include "cocos2d.h"
#include "string"
#include "Question.h"

class GlobalData {
public:
    static GlobalData* sharedGlobalData();
    static void purgeGlobalData();
    
    void setCurrentDifficult(Difficult dif) { currentDifficult_ = dif; }
    Difficult currentDifficult() const { return currentDifficult_; }
    
    void setCurrentGameId(const std::string& gameId) { currentGameId_ = gameId; }
    const std::string& currentGameId() const { return currentGameId_; }
protected:
    Difficult currentDifficult_;
    std::string currentGameId_;
};

#endif
