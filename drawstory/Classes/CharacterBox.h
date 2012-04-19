//
//  CharacterBox.h
//  drawstory
//
//  Created by 靖宇 张 on 12-4-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_CharacterBox_h
#define drawstory_CharacterBox_h

#include "cocos2d.h"

class CharacterBox : public cocos2d::CCSprite {
    enum { kTagCharacter = 1000 };
public:
    static CharacterBox* characterBoxWithSpriteFrameName(const char* frameName);
public:
    CharacterBox() : character_(' ') { }
    
    void setCharacter(char ch);
    char getCharacter();
    bool isSpace() const { return (character_ == ' '); }
protected:
    char character_;
};



#endif
