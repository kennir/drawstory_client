//
//  SolveScene_TitleBarLayer.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SolvingScene_TitleBarLayer.h"
#include "Game.h"
#include "UserProfile.h"
#include "GlobalData.h"
#include <iostream>
#include <iomanip>

using namespace cocos2d;

namespace SolvingScene {
    
    enum {
        kZLabel = 1,
    };
    
    enum { 
        kTagTurnLabel = 100,
        kTagDescriptLabel,
    };
    
    bool TitleBarLayer::init() {
        bool result = false;
        do {
            CC_BREAK_IF(!CCLayer::init());
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            
            CCSprite* titlebar = CCSprite::spriteWithSpriteFrameName("title_start");
            CC_BREAK_IF(!titlebar);
            titlebar->setPosition(CCPointMake(winSize.width * 0.5f, titlebar->getContentSize().height * 0.5f));
            addChild(titlebar);
            

            
            CCLabelTTF* descLabel = CCLabelTTF::labelWithString("", 
                                                                CCSizeMake(238.0f, 50.0f), 
                                                                CCTextAlignmentLeft, 
                                                                "Arial", 
                                                                12.0f);
            descLabel->setPosition(CCPointMake(190.0f, 55.0f));
            descLabel->setColor(ccWHITE);
            addChild(descLabel,kZLabel,kTagDescriptLabel);
            

            
            CCPoint turnPosition = CCPointMake(34.0f,40.0f);
            CCLabelTTF* turnLabel = CCLabelTTF::labelWithString("", "Arial-BoldMT", 24.0f);
            turnLabel->setPosition(turnPosition);
            turnLabel->setColor(ccBLACK);
            addChild(turnLabel,kZLabel,kTagTurnLabel);
            
            
            result = true;
        } while (0);
        return result;
    }
    
    void TitleBarLayer::onEnter() {
        CCLayer::onEnter();
        
        Game* game = UserProfile::sharedUserProfile()->findGame(GlobalData::sharedGlobalData()->currentGameId());
        CC_ASSERT(game != NULL);
        const Question& question = game->question();
        const Question::Word& word = question.word(GlobalData::sharedGlobalData()->currentDifficult());
        std::ostringstream oss;
        oss << game->otherPlayerName() << "正在为你绘制" << word.word;
        
        CCLabelTTF* desc = static_cast<CCLabelTTF*>(getChildByTag(kTagDescriptLabel));
        desc->setString(oss.str().c_str());
        
        oss.str("");
        oss << std::setw(2) << std::setfill('0') << (game->turn());
        
        CCLabelTTF* turn = static_cast<CCLabelTTF*>(getChildByTag(kTagTurnLabel));
        turn->setString(oss.str().c_str());
    }
    
}

    