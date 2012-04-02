//
//  types.h
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_types_h
#define drawstory_types_h


// lobby state
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


// event from logic
typedef enum 
{
    kLogicEventQueryGameForUserFinished,
} LogicEvent;

// priority of touch for layers
typedef enum {
    kTouchPriorityLobbyScene = 0,
    kTouchPriorityRegisterLayer = -1,
    kTouchPriorityPaintingLayer = -1,
    kTouchPriorityCanvasLayer = -2,
    kTouchPriorityMessageLayer = -2,

} TouchPriority;


// width of brush
typedef enum {
    kBrushWidth4 = 4,
    kBrushWidth8 = 8,
    kBrushWidth16 = 16,
    kBrushWidth32 = 32,
    
    kBrushMax = kBrushWidth32,
} BrushWidth;


#endif
