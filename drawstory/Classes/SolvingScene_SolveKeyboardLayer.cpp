//
//  SolvingScene_SolveKeyboardLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SolvingScene_SolveKeyboardLayer.h"
#include "Game.h"
#include "Question.h"
#include "UserProfile.h"
#include "GlobalData.h"
#include "types.h"
#include "CharacterBox.h"

using namespace cocos2d;

namespace SolvingScene {
    
    
    bool SolveKeyboardLayer::init() {
        bool result = false;
        do {
            CC_BREAK_IF(!KeyboardLayer::init());
            
            
            
            
            result = true;
        } while (0);
        return result;
    }
    
    void SolveKeyboardLayer::onEnter() {
        KeyboardLayer::onEnter();
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kTouchPriorityKeyboardLayer, true);
        
        
        // initialize candidate
        Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
        CC_ASSERT(game != NULL);
        const Question& question = game->question();
        const Question::Word& word = question.word(GlobalData::sharedGlobalData()->currentDifficult());
        
        candidate_ = answer_ = word.answer;
        
        // fill random word
        while(candidate_.size() < kMaxCandidate) {
            candidate_.push_back('a' + (rand() % 26));
        }
        
        CC_ASSERT(candidate_.size() == kMaxCandidate);
        
        std::random_shuffle(candidate_.begin(), candidate_.end());
        CCLOG("random candidta:%s",candidate_.c_str());
        
        // layout buttons 
        layoutCandidate();
        layoutAnswerCharacterFrame();
        
    }
    
    void SolveKeyboardLayer::onExit() {
        KeyboardLayer::onExit();
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    }
    
    
    bool SolveKeyboardLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        
        CCPoint localPos = convertTouchToNodeSpace(touch);
        
        do {
            
            // hit answer button
            if(hitTestWithAnswerCharacters(localPos))
                break;
            
            // hit candidate button if hit test is true check player answer
            if(!hitTestWithCandidateCharacters(localPos))
                break;
            
            if(checkPlayerAnswer()){
                // test code
                CCLabelTTF* descLabel = CCLabelTTF::labelWithString("Win Test", 
                                                                    CCSizeMake(210.0f, 380.0f), 
                                                                    CCTextAlignmentLeft, 
                                                                    "Arial", 
                                                                    30.0f);
                descLabel->setPosition(CCPointMake(205.0f, 380.0f));
                descLabel->setColor(ccRED);
                addChild(descLabel,10,110);

            }
        } while (0);
        
        return true;
    }
    
    void SolveKeyboardLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        
    }
    
    void SolveKeyboardLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        
    }
    
    void SolveKeyboardLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        
    }
 
    
    bool SolveKeyboardLayer::checkPlayerAnswer()
    {
        CharacterBox* characterBox;
        std::ostringstream oss;
        for (std::vector<CharacterBox*>::iterator iter = answerCharacters_.begin(); iter != answerCharacters_.end(); iter++) {
            characterBox = *iter;
            oss << characterBox->getCharacter();
        }
        CCLOG("checkPlayerAnswer answer=%s play=%s check=%d", answer_.c_str(),oss.str().c_str(),(answer_.compare(oss.str())) == 0 ? true : false);
        return (answer_.compare(oss.str())) == 0 ? true : false;
    }    

}


