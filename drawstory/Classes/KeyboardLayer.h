//
//  KeyboardLayer.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_KeyboardLayer_h
#define drawstory_KeyboardLayer_h

#include "cocos2d.h"

class CharacterBox;
class KeyboardLayer : public cocos2d::CCLayer {
public:
    enum { kMaxCandidate = 12 };
    enum { kCharPerLine = 6, kLines = 2 };
public:
    LAYER_NODE_FUNC(KeyboardLayer)
    
    virtual bool init();
    virtual void onExit();
protected:
    // create and layout candidate sprites
    void layoutCandidate();
    void layoutAnswerCharacterFrame();
    // create sprite
    CharacterBox* createCharacterButton(char ch) const;
    
    bool hitTestWithCandidateCharacters(const cocos2d::CCPoint& localPos);
    bool hitTestWithAnswerCharacters(const cocos2d::CCPoint& localPos);
protected:
    std::string candidate_;
    std::string answer_;
    // sprites
    std::vector<CharacterBox*> candidateCharacters_;
    std::vector<CharacterBox*> answerCharacters_;
};

#endif
