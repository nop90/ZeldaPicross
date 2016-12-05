/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __EVENT_H__
#define __EVENT_H__

struct Event {
    bool ACTION;
    bool HOLD_ACTION;
    bool FLAG;
    bool HOLD_FLAG;
    bool HYPOTHESE;
    bool FULLSCREEN;
    bool QUIT;
    bool ESCAPE;
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
    bool RETURN;
    bool ERROR;
    bool CANCEL_ACTION;
    bool CANCEL_FLAG;
    bool MOUSE_ACTION;
    bool MOUSE_HOLD_ACTION;
    bool MOUSE_FLAG;
    bool MOUSE_HOLD_FLAG;
    bool MOUSE_CANCEL_ACTION;
    bool MOUSE_CANCEL_FLAG;
    bool MOUSE_ON_OFF;
    bool MUSIC_ON_OFF;
};

#endif  // Event.h
