/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"

class Options {
    public :
        static Options* getInstance();
        
        void init(int m, int s);
        void start();
        void handleEvent(Event* event);
        void draw(SDL_Surface* gpScreen);
    private :
        static Options instance;
        
        Options();
        ~Options();
        
        void cadre(int x, int y, int w, int h);
        
        int line;
        int column;
        int volume;
        int volson;
        int volumeTmp; // can be cancelled
        int volsonTmp; // can be cancelled
        SDL_Surface* image;
        SDL_Surface* curseurLink;
        SDL_Surface* imageCadre;
};

#endif  // Menu.h
