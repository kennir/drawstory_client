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
            CharacterBox* button = createCharacterButton(candidate_[index]);
            button->setPosition(position);
            addChild(button,1,CANDIDATE_INDEX_TO_TAG(index));
            
            candidateCharacters_.push_back(button);
            
            position.x += 44.0f;
        }
        position.x = 32.0f;
        position.y -= 44.0f;
    }
}

CharacterBox* KeyboardLayer::createCharacterButton(char ch) const {
    // create button border first
    CharacterBox* border = CharacterBox::characterBoxWithSpriteFrameName("button");
    CC_ASSERT(border != NULL);
    
    // create words
    std::ostringstream oss;
    oss << "w_" << ch;
    CCSprite* character = CCSprite::spriteWithSpriteFrameName(oss.str().c_str());
    CC_ASSERT(character != NULL);
    
    CCSize buttonSize = border->getContentSize();
    character->setPosition(CCPointMake(buttonSize.width * 0.5f, buttonSize.height * 0.5f));
    
    border->setCharacter(ch);
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

bool KeyboardLayer::hitTestWithCandidateCharacters(const cocos2d::CCPoint& localPos)
{
    CharacterBox* candidate;
    bool loopEnd = false;
    
    // check all candidate
    for (std::vector<CharacterBox*>::iterator candidateIter = candidateCharacters_.begin(); !loopEnd && candidateIter != candidateCharacters_.end(); candidateIter++) {
        
        candidate = *candidateIter;
        
        // check
        if(CCRect::CCRectContainsPoint(candidate->boundingBox(), localPos))
        {
            // check first space answer button
            for (std::vector<CharacterBox*>::iterator answerIter = answerCharacters_.begin(); !loopEnd && answerIter != answerCharacters_.end(); answerIter++) {
                
                CharacterBox* answerBox = *answerIter;
                if(answerBox != NULL && answerBox->isSpace())
                {
                    answerBox->setCharacter(candidate->getCharacter());
                    candidateCharacters_.erase(candidateIter);
                    removeChild(candidate, true);
                    CCLOG("hitTestWithCandidateCharacters candidateCharacters_=%d",candidateCharacters_.capacity());
                    CCLOG("hitTestWithCandidateCharacters answerCharacters_=%d",answerCharacters_.capacity());
                    loopEnd = true;
                }
            }
            return true;
        }
    }
    return false;
}

bool KeyboardLayer::hitTestWithAnswerCharacters(const cocos2d::CCPoint& localPos)
{
    CharacterBox* answerBox;
    CCPoint position = CCPointMake(32.0f, 70.0f);
    bool loopEnd = false;
    
    // check all answer button
    for (std::vector<CharacterBox*>::iterator iter = answerCharacters_.begin(); !loopEnd && iter != answerCharacters_.end(); iter++) {
        
        answerBox = *iter;
        
        if(CCRect::CCRectContainsPoint(answerBox->boundingBox(), localPos) && (answerBox->getCharacter() != ' '))
        {
            int index = 0;
            
            //renew a characterbox
            for(int line = 0; line < kLines && !loopEnd; ++line) {
                for(int ch = 0; ch < kCharPerLine && !loopEnd; ++ch,++index) {
                    CharacterBox* button = static_cast<CharacterBox*>(getChildByTag(CANDIDATE_INDEX_TO_TAG(index)));
                    if(button == NULL)
                    {
                        button = createCharacterButton(answerBox->getCharacter());
                        position.x += ch * 44.0f;
                        position.y -= (index-ch)/kCharPerLine*44.0f;
                        button->setPosition(position);
                        addChild(button,1,CANDIDATE_INDEX_TO_TAG(index));
                        candidateCharacters_.push_back(button);
                        CCLOG("hitTestWithAnswerCharacters candidateCharacters_=%d",candidateCharacters_.capacity());
                        CCLOG("hitTestWithAnswerCharacters answerCharacters_=%d",answerCharacters_.capacity());
                        loopEnd = true;
                    }
                }
            }
            answerBox->setCharacter(' ');
            CCLOG("hitTestWithAnswerCharacters");
            return true;
        }
    }
    return false;
}

void KeyboardLayer::onExit() {
    CCLayer::onExit();
    candidate_.clear();
    answer_.clear();
    candidateCharacters_.clear();
    answerCharacters_.clear();
}




