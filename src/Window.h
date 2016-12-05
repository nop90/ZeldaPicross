/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"
#include "Game.h"

class Window {
    public :
        Window(bool full);
        ~Window();
        void loop();
    private :
        bool handleEvent(Event* event, Game* gpGame);
        void toggleFullScreen();
        
        bool fullscreen;
        bool zoom;
        
        SDL_Surface* gpScreen;        
};

#endif  // Window.h
