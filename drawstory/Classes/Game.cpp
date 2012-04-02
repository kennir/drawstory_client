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



void Game::updateFromJson(const Json::Value &game) {
    // update email for player
    std::string anthorEmail = game["opponent"].asString();
    if(isOwner_){
        ownerName_ = UserProfile::sharedUserProfile()->email();
        opponentName_ = anthorEmail;
    } else {
        ownerName_ = anthorEmail;
        opponentName_ = UserProfile::sharedUserProfile()->email();
    }
    
    // update game detail information
    const Json::Value& detail = game["detail"];
    
    // state
    objectId_ = detail["_id"].asString();
    state_ = static_cast<GameState>(detail["state"].asInt());
    turn_ = detail["turn"].asInt();
    ownerObjectId_ = detail["owner"].asString();
    opponentObjectId_ = detail["opponent"].asString();
}


std::string Game::stateString() const {
    std::string message;
    if(isOwner()) {
        switch (state()) {
            case kGameStateWaitingOwnerDraw:
                message = "绘图回合";
                break;
            case kGameStateWaitingOpponentAnswer:
                message = "等待中";
                break;
            default:
                break;
        }
    } else {
        switch (state()) {
            case kGameStateWaitingOwnerDraw:
                message = "等待中";
                break;
            case kGameStateWaitingOpponentAnswer:
                message = "答题回合";
                break;
            default:
                break;
        }
    }
    return message;
}


