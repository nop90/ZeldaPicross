/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Cadre.h"
#include "Resources.h"

Cadre Cadre::instance=Cadre();

Cadre::Cadre() : image(0) {
}

Cadre::~Cadre() {
}

Cadre* Cadre::getInstance() {
    return &instance;
}

void Cadre::init() {
    image = Resources::getInstance()->getCadre();
}

void Cadre::drawCadre(SDL_Surface* gpScreen, int x, int y, int w, int h, int theme) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = 16; 
    src.h = 16;
    
    src.x = 0 + theme * 48; src.y = 0; dst.x = x; dst.y = y;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    src.x = 16 + theme * 48; src.y = 0;
    for (int i = 16; i < w - 16; i += 16) {
        dst.x = x + i; dst.y = y;
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    src.x = 32 + theme * 48; src.y = 0; dst.x = x + w - 16; dst.y = y;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    src.x = 0 + theme * 48; src.y = 16;
    for (int j = 16; j < h - 16; j += 16) {
        dst.x = x; dst.y = y + j;
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    src.x = 16 + theme * 48; src.y = 16;
    for (int i = 16; i < w - 16; i += 16) {
        for (int j = 16; j < h - 16; j += 16) {
            dst.x = x + i; dst.y = y + j;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
        }
    }
    
    src.x = 32 + theme * 48; src.y = 16;
    for (int j = 16; j < h - 16; j += 16) {
        dst.x = x + w - 16; dst.y = y + j;
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    src.x = 0 + theme * 48; src.y = 32; dst.x = x; dst.y = y + h - 16;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    src.x = 16 + theme * 48; src.y = 32;
    for (int i = 16; i < w - 16; i += 16) {
        dst.x = x + i; dst.y = y + h - 16;
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    src.x = 32 + theme * 48; src.y = 32; dst.x = x + w - 16; dst.y = y + h - 16;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
}
