//
//  Game.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-1.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Game.h"
#include "UserProfile.h"

Game* Game::gameFromJson(const Json::Value &json){
    Game* g = new Game();
    g->updateFromJson(json);
    return g;
}

bool Game::isMyTurn() const {
    bool myTurn = false;
    if(isOwner() && (state() == kGameStateOwnerTurn))
        myTurn = true; 
    else if(!isOwner() && (state() == kGameStateOpponentTurn))
        myTurn = true;
    
    return myTurn;
}


void Game::updateFromJson(const Json::Value &game) {
    // update email for player
    
    // update game detail information
    const Json::Value& detail = game["detail"];
    
    // state
    objectId_ = detail["_id"].asString();
    state_ = static_cast<GameState>(detail["state"].asInt());
    turn_ = detail["turn"].asInt();
    ownerObjectId_ = detail["owner"].asString();
    opponentObjectId_ = detail["opponent"].asString();
    
    isOwner_ = (ownerObjectId_ == UserProfile::sharedUserProfile()->objectId());
    
    
    std::string anthorEmail = game["opponent"].asString();
    if(isOwner_){
        ownerName_ = UserProfile::sharedUserProfile()->email();
        opponentName_ = anthorEmail;
    } else {
        ownerName_ = anthorEmail;
        opponentName_ = UserProfile::sharedUserProfile()->email();
    }
    


}


std::string Game::stateString() const {
    std::string message;
    if(isMyTurn()) {
        message = "点击开始游戏";
    } else {
        message = "等待对方回合";
    }
    return message;
}


