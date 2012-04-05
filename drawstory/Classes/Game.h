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

typedef struct _DataForSend {
    SendState state;
    std::string base64Data;
    _DataForSend() : state(kSendStateIdle) { }
} DataForSend;

class Game 
{
public:
    static Game* gameFromJson(const Json::Value& json);
public:
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
    
    // 是否存在答题的录像
    bool hasAnswerReplay() const { return false; }
    // 是否需要回答对手的问题
    bool hasPaintingReplay() const { return false; }

    const Question& question() const { return question_; }
    

    
    // there has data for send
    bool allDataSent() const { return (!hasDataForWaiting() && !hasDataForSending()); }
    bool hasDataForWaiting() const { return (paintingRecord_.state == kSendStateWaiting); }
    bool hasDataForSending() const { return (paintingRecord_.state == kSendStateSending); }
    
    
    void savePaintingRecord(Difficult diff,const std::string& record);
    void beginSendPaintingRecord() { paintingRecord_.state = kSendStateSending; }
    void paintingRecordSent(bool result);
    const DataForSend& paintingRecord() const { return paintingRecord_; }
protected:
    GameState state_;
    Drawer drawer_;                 // who draw the question
    bool isOwner_;  // is owner
    int turn_;                      // current turn

    std::string objectId_;          // objectId of game
    std::string ownerObjectId_;     // objectId for owner
    std::string ownerName_;         // owner name
    std::string opponentObjectId_;  // ObjectId for opponent
    std::string opponentName_;      // opponent name
    
    Question question_;
    
    DataForSend paintingRecord_; // compressed painting record
};


#endif
