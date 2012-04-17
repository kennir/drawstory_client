//
//  KeyboardLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "KeyboardLayer.h"
#include "CharacterBox.h"


using namespace cocos2d;




#define CANDIDATE_INDEX_TO_TAG(i)               ((i) + 1000)
#define ANSWER_FRAME_INDEX_TO_TAG(i)            ((i) + 2000)
#define SPACE                                   (4.0f)


bool KeyboardLayer::init() {
    bool result = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite* background = CCSprite::spriteWithSpriteFrameName("keyboard");
        CC_BREAK_IF(!background);
        background->setPosition(CCPointMake(winSize.width * 0.5f, background->getContentSize().height * 0.5f));
        addChild(background);
        
        result = true;
    } while (0);
    return result;
}


void KeyboardLayer::layoutCandidate() {
    CC_ASSERT(!candidate_.empty());
    
    CCPoint position = CCPointMake(32.0f, 70.0f);
    
    int index = 0;
    
    for(int line = 0; line < kLines; ++line) {
        for(int ch = 0; ch < kCharPerLine; ++ch,++index) {
            CCSprite* button = createCharacterButton(candidate_[index]);
            button->setPosition(position);
            addChild(button,1,CANDIDATE_INDEX_TO_TAG(index));
            
            candidateCharacters_.push_back(button);
            
            position.x += 44.0f;
        }
        position.x = 32.0f;
        position.y -= 44.0f;
    }
}

CCSprite* KeyboardLayer::createCharacterButton(char ch) const {
    // create button border first
    CCSprite* border = CCSprite::spriteWithSpriteFrameName("button");
    CC_ASSERT(border != NULL);
    // create words
    std::ostringstream oss;
    oss << "w_" << ch;
    CCSprite* character = CCSprite::spriteWithSpriteFrameName(oss.str().c_str());
    CC_ASSERT(character != NULL);
    
    CCSize buttonSize = border->getContentSize();
    character->setPosition(CCPointMake(buttonSize.width * 0.5f, buttonSize.height * 0.5f));
    
    border->addChild(character);
    
    return border;
}

void KeyboardLayer::layoutAnswerCharacterFrame() {
    CC_ASSERT(!answer_.empty());
    // computer length needed
    float length = static_cast<float>(answer_.size()) * 36.0f;
    if(answer_.size() > 1) 
        length += (answer_.size() - 1) * SPACE;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint startPos = CCPointMake( (winSize.width - length) * 0.5f + 18.0f , 120.0f);
    
    for(int index = 0; index < answer_.size(); ++index) {
        CharacterBox* cb = CharacterBox::characterBoxWithSpriteFrameName("button_w");
        cb->setPosition(startPos);
        addChild(cb,1,ANSWER_FRAME_INDEX_TO_TAG(index));
        
        // put to vector
        answerCharacters_.push_back(cb);
                
        startPos.x += (36.0f + SPACE);
    }
}

void KeyboardLayer::onExit() {
    CCLayer::onExit();
    candidate_.clear();
    answer_.clear();
    candidateCharacters_.clear();
    answerCharacters_.clear();
}




