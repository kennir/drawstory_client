//
//  Question.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_Question_h
#define drawstory_Question_h

#include <string>
#include "Json/json.h"

typedef enum {
    kDifficultEasy = 0,
    kDifficultNormal,
    kDifficultHard,
    kDifficultCount
} Difficult;

class Question {
public:
    typedef struct _W {
        Difficult difficult;
        std::string word;
        std::string answer;
        std::string prompt;
    } Word;
public:
    Word& word(Difficult diff) { return words_[diff]; }
    const Word& word(Difficult diff) const { return words_[diff]; }
    
    const std::string& paintingId() const { return paintingId_; }
    const std::string& answeringId() const { return answeringId_; }
    
    void setDifficult(Difficult dif) { difficult_ = dif; }
    Difficult difficult() const { return difficult_; }

    void updateFromJson(const Json::Value& question);
protected:
    Word words_[kDifficultCount];
    Difficult difficult_;   // difficult used
    std::string paintingId_;
    std::string answeringId_;
};


#endif
