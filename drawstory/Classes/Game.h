//
//  Game.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-1.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_Game_h
#define drawstory_Game_h

#include "cocos2d.h"
#include "json/json.h"

typedef enum {
    kGameStateWaitingOpponent = 0,
    kGameStateWaitingOwnerDraw,
    kGameStateWaitingOpponentAnswer,
    kGameStateAnswerUploaded,
} GameState;

class Game 
{
public:
    static Game* gameFromJson(const Json::Value& json);
public:
    const std::string& objectId() const { return objectId_; }
    
    bool isOwner() const { return isOwner_; }
    
    const std::string& otherPlayerName() const { 
        return (isOwner()) ? ownerName() : opponentName();
    }
    
    GameState state() const { return state_; }
    void setState(GameState newState) { state_ = newState; }
    
    void setOwnerObjectId(const std::string& objectId) { ownerObjectId_ = objectId; }
    void setOwnerName(const std::string& name) { ownerName_ = name; }
    
    const std::string& ownerObjectId() const { return ownerObjectId_; }
    const std::string& ownerName() const { return ownerName_; }
    
    void setOpponentObjectId(const std::string& objectId) { opponentObjectId_ = objectId; }
    void setOpponentName(const std::string& name) { opponentName_ = name; }
    
    const std::string& opponentObjectId() const { return opponentObjectId_; }
    const std::string& opponentName() const { return opponentName_; }
    
    std::string stateString() const;
    
    int turn() const { return turn_; }
    void setTurn(int newTurn) { turn_ = newTurn; }
    
    void updateFromJson(const Json::Value& game);
protected:
    GameState state_;
    int turn_;                      // current turn
    std::string objectId_;          // objectId of game
    std::string ownerObjectId_;     // objectId for owner
    std::string ownerName_;         // owner name
    std::string opponentObjectId_;  // ObjectId for opponent
    std::string opponentName_;      // opponent name
    
    bool isOwner_;  // is owner
};


#endif
