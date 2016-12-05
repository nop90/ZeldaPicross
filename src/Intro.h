/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __INTRO_H__
#define __INTRO_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"

class Intro {
    public :
        static Intro* getInstance();
        
        void init();
        void start();
        void handleEvent(Event* event);
        void draw(SDL_Surface* gpScreen);
    private :
        static Intro instance;
        
        Intro();
        ~Intro();
        
        int anim;
        Uint32 lastAnimTime;
        SDL_Surface* imageStep[3];
        SDL_Surface* imageFond1;
        SDL_Surface* imageFond2;
};

#endif  // Intro.h
