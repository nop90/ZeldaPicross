/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RECORDS_H__
#define __RECORDS_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"

class Records {
    public :
        static Records* getInstance();
        
        void init();
        void start(int r, int q, int f, int qt, int ft);
        void handleEvent(Event* event);
        void draw(SDL_Surface* gpScreen);
    private :
        static Records instance;
        
        Records();
        ~Records();
        
        void cadre(int x, int y, int w, int h, SDL_Surface* im);
        
        int column;
        int rank;
        int quest;
        int free;
        int questTime;
        int freeTime;
        bool erase;
        int line;
        SDL_Surface* image;
        SDL_Surface* imageErase;
        SDL_Surface* curseurLink;
        SDL_Surface* imageCadre;
        SDL_Surface* curseurFee;
};

#endif  // Records.h
