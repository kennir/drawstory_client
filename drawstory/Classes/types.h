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
    kTouchPriorityGameListLayer = 0,
    kTouchPriorityRegisterLayer = -1, 
    kTouchPriorityMessageLayer = -2,

    kTouchPriorityPaintingLayer = -1,
    kTouchPriorityCanvasLayer = -2,
    kTouchPriorityTitleBarLayer = -3,
    kTouchPriorityCommandBarLayer = -3,

} TouchPriority;


// width of brush
typedef enum {
    kBrushWidth1 = 4,
    kBrushWidth2 = 8,
    kBrushWidth3 = 16,
    kBrushWidth4 = 32,
    
    kBrushMax = kBrushWidth4,
} BrushWidth;


// draw mode
typedef enum {
    kPaintModeDraw,
    kPaintModeErase
} PaintMode;


#endif
