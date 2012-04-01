//
//  types.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_types_h
#define drawstory_types_h

typedef enum 
{
    kLobbyStateUnhandle,
    kLobbyStateRegisterUser,
    kLobbyStateWaitingForRegisterUser,
    kLobbyStateLoginUser,
    kLobbyStateWaitingForLoginUser,
    kLobbyStateIdle,
    kLobbyStateWaitingForCreateRandomGame,
} LobbyState;

typedef enum 
{
    kLogicEventQueryGameForUserFinished,
} LogicEvent;

#endif
