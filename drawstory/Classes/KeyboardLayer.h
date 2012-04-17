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
    cocos2d::CCSprite* createCharacterButton(char ch) const;
    // return -1 indicate not hitted
    int hitTestWithCandidateCharacters(const cocos2d::CCPoint& localPos);
    
protected:
    std::string candidate_;
    std::string answer_;
    // sprites
    std::vector<cocos2d::CCSprite*> candidateCharacters_;
    std::vector<CharacterBox*> answerCharacters_;
};

#endif
