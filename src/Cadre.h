/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CADRE_H__
#define __CADRE_H__

#include "3ds/SDL_3ds.h"

class Cadre {
    public :
        static Cadre* getInstance();
        void drawCadre(SDL_Surface* gpScreen, int x, int y, int w, int h, int theme = 0);
        void init();
    private :
        Cadre();
        ~Cadre();
        static Cadre instance;
        SDL_Surface* image;
};

#endif  // Cadre.h
