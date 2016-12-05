/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Options.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"

Options Options::instance=Options();

Options::Options() {
    image = 0;
    volume = 0;
    volson = 0;
}

Options::~Options() {
    SDL_FreeSurface(image);
}

Options* Options::getInstance() {
    return &instance;
}

void Options::init(int m, int s) {
    volume = m;
    volson = s;
    curseurLink = Resources::getInstance()->getLinkMenu();
    imageCadre = Resources::getInstance()->getCadreMenu();
    Audio::getInstance()->setVolume(volume);
    Audio::getInstance()->setVolson(volson);
}

void Options::start() {
    Audio::getInstance()->playMusic(190);
    volumeTmp = volume;
    volsonTmp = volson;
    line = 2;
    column = 0;
    
    
    // build background
    if (image != NULL) SDL_FreeSurface(image);
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 16; src.y = 16; src.w = 16; src.h = 16;
    for (int j = 0; j < 240; j+=16) {
        for (int i = 0; i < 320; i+=16) {
            dst.x = i; dst.y = j;
            SDL_BlitSurface(imageCadre, &src, image, &dst);
        }
    }
    
    // title
    string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 3);
    int width = text.length() * 6;
    
    src.x = 0; src.y = 0; dst.x = 16; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 0; src.y = 16; dst.x = 16; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 0; src.y = 32; dst.x = 16; dst.y = 32;
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    for (int i = 0; i <= width / 16; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    int newX = 48+16*((int)(width/16));
    src.x = 32; src.y = 0; dst.x = newX; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = newX; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = newX; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    Text::getInstance()->affiche(image, text, 32, 16);
    
    
    // fields
    cadre(16,64,288,32);
    cadre(16,128,288,32);
    cadre(16,192,136,32);
    cadre(168,192,136,32);
    
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 5);
    Text::getInstance()->affiche(image, text, 63, 72);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 6);
    Text::getInstance()->affiche(image, text, 63, 136);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 7);
    Text::getInstance()->affiche(image, text, 63, 200);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 8);
    Text::getInstance()->affiche(image, text, 215, 200);
    
    src.x = 0; src.y = 96; dst.x = 128; dst.y = 72; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    for (int i = 144; i < 264; i+=16) {
        src.x = 8; src.y = 96; dst.x = i; dst.y = 72; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    src.x = 16; src.y = 96; dst.x = 264; dst.y = 72; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    src.x = 0; src.y = 96; dst.x = 128; dst.y = 136; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    for (int i = 144; i < 264; i+=16) {
        src.x = 8; src.y = 96; dst.x = i; dst.y = 136; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    src.x = 16; src.y = 96; dst.x = 264; dst.y = 136; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
}

void Options::cadre(int x, int y, int w, int h) {
    
    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    
    for (int j = y+16; j < y+h-16; j+=16)
        for (int i = x+16; i < x+w-16; i+=16) {
            src.x = 16; src.y = 64; dst.x = i; dst.y = j; 
            SDL_BlitSurface(imageCadre, &src, image, &dst);
        }
    
    //haut et bas
    for (int i = x+16; i < x+w-16; i+=16) {
        src.x = 16; src.y = 48; dst.x = i; dst.y = y; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 80; dst.x = i; dst.y = y+h-16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    //gauche et droite
    for (int j = y+16; j < y+h-16; j+=16) {
        src.x = 0; src.y = 64; dst.x = x; dst.y = j; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 32; src.y = 64; dst.x = x+w-16; dst.y = j;
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    //haut gauche
    src.x = 0; src.y = 48; dst.x = x; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //haut droite
    src.x = 32; src.y = 48; dst.x = x+w-16; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //bas gauche
    src.x = 0; src.y = 80; dst.x = x; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //bas droite
    src.x = 32; src.y = 80; dst.x = x+w-16; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
}

void Options::handleEvent(Event* event) {
    if (event->RETURN) {
        if (line == 2) {
            if (column == 0) {
                volume = volumeTmp;
                volson = volsonTmp;
                Game::getInstance()->setVolume(volume);
                Game::getInstance()->setVolson(volson);
                Game::getInstance()->saveSystem();
                Audio::getInstance()->playSound(2);
            } else if (column == 1) {
                Audio::getInstance()->setVolume(volume);
                Audio::getInstance()->setVolson(volson);
                Audio::getInstance()->playSound(2);
            }
            Game::getInstance()->setMode(MENU);
        }
        return;
    }
    
    if (event->UP) {
        line--;
        if (line < 0) {
            line = 2;
        }
        column = 0;
        Audio::getInstance()->playSound(3);
    }
    if (event->DOWN) {
        line++;
        if (line > 2) {
            line = 0;
        }
        column = 0;
        Audio::getInstance()->playSound(3);
    }
    if (event->LEFT) {
        switch (line) {
            case 0 :
                volumeTmp-=8;
                if (volumeTmp < 0) {
                    volumeTmp = 0;
                }
                Audio::getInstance()->setVolume(volumeTmp);
                break;
            case 1 :
                volsonTmp-=8;
                if (volsonTmp < 0) {
                    volsonTmp = 0;
                }
                Audio::getInstance()->setVolson(volsonTmp);
                break;
            case 2 :
                column--;
                if (column < 0) {
                    column = 1;
                }
                break;
        }
        Audio::getInstance()->playSound(3);
    }
    if (event->RIGHT) {
        switch (line) {
            case 0 :
                volumeTmp+=8;
                if (volumeTmp > 64) {
                    volumeTmp = 64;
                }
                Audio::getInstance()->setVolume(volumeTmp);
                break;
            case 1 :
                volsonTmp+=8;
                if (volsonTmp > 64) {
                    volsonTmp = 64;
                }
                Audio::getInstance()->setVolson(volsonTmp);
                break;
            case 2 :
                column++;
                if (column > 1) {
                    column = 0;
                }
                break;
        }
        Audio::getInstance()->playSound(3);
    }
}

void Options::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.w = 320; src.h = 240;
    dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    //curseurLink
    src.x = 0; src.y = 0; src.w = 16; src.h = 21;
    dst.x = 26 + 152 * column; dst.y = dst.y = 69 + 64 * line;
    SDL_BlitSurface(curseurLink, &src, gpScreen, &dst);
    
    src.h = 16; src.w = 8; src.x = 32; src.y = 96;
    dst.x = 136 + 2 * volumeTmp; dst.y = 56 + 16;
    SDL_BlitSurface(imageCadre, &src, gpScreen, &dst);
                
    dst.x = 136 + 2 * volsonTmp; dst.y = 56 + 16 + 64;
    SDL_BlitSurface(imageCadre, &src, gpScreen, &dst);
}
