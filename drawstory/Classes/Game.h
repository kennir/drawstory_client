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
#include "Question.h"
#include "json/json.h"
#include "Replay.h"

typedef enum {
    kGameStateWaitingOpponent = 0,
    kGameStateOwnerTurn,
    kGameStateOpponentTurn,
    kGameStateUnknown,                  // Waiting from server
} GameState;

typedef enum {
    kDrawerOwner        = 0,
    kDrawerOpponent,
} Drawer;

typedef enum {
    kSendStateWaiting,
    kSendStateSending,
    kSendStateIdle,
} SendState;

class Game 
{
public:
    static Game* gameFromJson(const Json::Value& json);
public:
    Game();
    
    
    const std::string& objectId() const { return objectId_; }
    
    bool isOwner() const { return isOwner_; }
    
    const std::string& otherPlayerName() const { 
        return (isOwner()) ? opponentName() : ownerName();
    }
    
    bool isMyTurn() const;
    
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
    

    const Question& question() const { return question_; }
    

    
    // there has data for send
    SendState replaySendState() const { return replaySendState_; }
    bool replaySent() const { return (replaySendState_ == kSendStateIdle); }

    void beginSendReplay() { replaySendState_ = kSendStateSending; }
    void endSendReplay(bool result);
    
    void commitTurn(Difficult diff,
                    const std::string& paintRecord,
                    size_t paintRecordSize,
                    const std::string& solveRecord,
                    size_t solveRecordSize);
    
    void receiveReplay(const std::string& paintReplay,size_t paintReplaySize,const std::string& solveReplay,size_t solveReplaySize) {
        replay_.setPaintReplay(paintReplay, paintReplaySize);
        replay_.setSolveReplay(solveReplay, solveReplaySize);
    }
    

    const Replay& replay() const { return replay_; }
   
protected:
    int turn_;                      // current turn
    GameState state_;
    bool isOwner_;  // is owner
    

    std::string objectId_;          // objectId of game
    std::string ownerObjectId_;     // objectId for owner
    std::string ownerName_;         // owner name
    std::string opponentObjectId_;  // ObjectId for opponent
    std::string opponentName_;      // opponent name
    
    Question question_;
    
    SendState replaySendState_;
    Replay replay_;
};


#endif
