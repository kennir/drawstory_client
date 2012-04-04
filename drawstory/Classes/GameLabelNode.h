//
//  GameLabelNode.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_GameLabelNode_h
#define drawstory_GameLabelNode_h

#include "cocos2d.h"

class Game;
class GameLabelNode : public cocos2d::CCNode {
public:
    static cocos2d::CCNode* node();
public:
    virtual bool init();
    void setGame(const Game* game);
    
    bool onTouchBegan(const cocos2d::CCPoint& localPos);
    void onTouchEnded(const cocos2d::CCPoint& localPos);
    void onTouchMoved(const cocos2d::CCPoint& localPos);
    void onTouchCancelled(const cocos2d::CCPoint& localPos);
protected:
    cocos2d::CCNode* hitTest(const cocos2d::CCPoint& localPos);
    Game* findGame() const;
    void onNodeClicked(cocos2d::CCNode* button);
protected:
    std::string gameId_;
    cocos2d::CCNode* trackingNode_;
};


#endif
