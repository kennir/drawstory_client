//
//  Question.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Question.h"
#include "cocos2d.h"

void Question::updateFromJson(const Json::Value &question) {
    const Json::Value& words = question["words"];
    for(int diff = 0; diff < kDifficultCount; ++diff) {
        const Json::Value& word = words[diff];
    
        int difficult = word["difficult"].asInt();
        words_[difficult].difficult = static_cast<Difficult>(difficult);
        words_[difficult].word = word["word"].asString();
        words_[difficult].answer = word["answer"].asString();
        words_[difficult].prompt = word["prompt"].asString();
    }
    
    // get painting id
    paintingId_ = question["paintingid"].asString();
    answeringId_ = question["answeringid"].asString();
}

