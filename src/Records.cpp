/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Records.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"

Records Records::instance=Records();

Records::Records() {
    image = 0;
    imageErase = 0;
}

Records::~Records() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(imageErase);
}

Records* Records::getInstance() {
    return &instance;
}

void Records::init() {
    curseurLink = Resources::getInstance()->getLinkMenu();
    imageCadre = Resources::getInstance()->getCadreMenu();
    curseurFee = Resources::getInstance()->getFeeMenu();
}

void Records::start(int r, int q, int f, int qt, int ft) {
    Audio::getInstance()->playMusic(190);
    
    rank = r;
    quest = q;
    free = f;
    questTime = qt;
    freeTime = ft;
    column = 0;
    erase = false;
    line = 1;
    
    //erase
    if (imageErase == NULL) {
        imageErase = SDL_CreateRGBSurface(SDL_HWSURFACE, 112, 72, 32, 0, 0, 0, 0);
        cadre(0, 0, 112, 72, imageErase);
        string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 16);
        Text::getInstance()->affiche(imageErase, text, 29, 9);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 17);
        Text::getInstance()->affiche(imageErase, text, 47, 29);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 18);
        Text::getInstance()->affiche(imageErase, text, 47, 49);
    }
    
    
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
    string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 4);
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
    cadre(16,48,288,128,image);
    cadre(16,192,136,32,image);
    cadre(168,192,136,32,image);
    
    ostringstream oss;
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 9);
    if (rank != -1) {
        char c = 64 + NB_RANK - r;
        oss << c;
        text += oss.str();
    } else {
        text += "-";
    }
    Text::getInstance()->affiche(image, text, 24, 72 - 16);
    
    
    oss.str("");
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 10);
    oss << q;
    text += oss.str();
    Text::getInstance()->affiche(image, text, 24, 72 + 16);
    
    oss.str("");
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 11);
    oss << f;
    text += oss.str();
    Text::getInstance()->affiche(image, text, 24, 72 + 16 * 2);
    
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 12);
    if (q == 200 && qt < 360000) {
        text += Text::getInstance()->getTime(qt);
    } else {
        text += "-";
    }
    Text::getInstance()->affiche(image, text, 24, 72 + 16 * 4);
    
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 13);
    if (f == 200 && ft < 360000) {
        text += Text::getInstance()->getTime(ft);
    } else {
        text += "-";
    }
    Text::getInstance()->affiche(image, text, 24, 72 + 16 * 5);
    
    
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 14);
    Text::getInstance()->affiche(image, text, 63, 200);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 15);
    Text::getInstance()->affiche(image, text, 215, 200);
}

void Records::cadre(int x, int y, int w, int h, SDL_Surface* im) {
    
    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    
    for (int j = y+16; j < y+h-16; j+=16)
        for (int i = x+16; i < x+w-16; i+=16) {
            src.x = 16; src.y = 64; dst.x = i; dst.y = j; 
            SDL_BlitSurface(imageCadre, &src, im, &dst);
        }
    
    //haut et bas
    for (int i = x+16; i < x+w-16; i+=16) {
        src.x = 16; src.y = 48; dst.x = i; dst.y = y; 
        SDL_BlitSurface(imageCadre, &src, im, &dst);
        src.x = 16; src.y = 80; dst.x = i; dst.y = y+h-16; 
        SDL_BlitSurface(imageCadre, &src, im, &dst);
    }
    
    //gauche et droite
    for (int j = y+16; j < y+h-16; j+=16) {
        src.x = 0; src.y = 64; dst.x = x; dst.y = j; 
        SDL_BlitSurface(imageCadre, &src, im, &dst);
        src.x = 32; src.y = 64; dst.x = x+w-16; dst.y = j;
        SDL_BlitSurface(imageCadre, &src, im, &dst);
    }
    
    //haut gauche
    src.x = 0; src.y = 48; dst.x = x; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, im, &dst);
    
    //haut droite
    src.x = 32; src.y = 48; dst.x = x+w-16; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, im, &dst);
    
    //bas gauche
    src.x = 0; src.y = 80; dst.x = x; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, im, &dst);
    
    //bas droite
    src.x = 32; src.y = 80; dst.x = x+w-16; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, im, &dst);
}

void Records::handleEvent(Event* event) {
    if (event->RETURN) {
        if (!erase) {
            if (column == 0) {
                Audio::getInstance()->playSound(2);
                Game::getInstance()->setMode(MENU);
            } else if (column == 1) {
                erase = true;
                line = 1;
                Audio::getInstance()->playSound(1);
            }
        } else {
            if (line == 0) {
                Game::getInstance()->razRecords();
                column = 1; // raz restart records
            }
            Audio::getInstance()->playSound(2);
            erase = false;
        }
        return;
    }
    
    if (event->LEFT && !erase) {
        column--;
        if (column < 0) {
            column = 1;
        }
        Audio::getInstance()->playSound(3);
    }
    if (event->RIGHT && !erase) {
        column++;
        if (column > 1) {
            column = 0;
        }
        Audio::getInstance()->playSound(3);
    }
    if (event->UP && erase) {
        line--;
        if (line < 0) {
            line = 1;
        }
        Audio::getInstance()->playSound(3);
    }
    if (event->DOWN && erase) {
        line++;
        if (line > 1) {
            line = 0;
        }
        Audio::getInstance()->playSound(3);
    }
}

void Records::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.w = 320; src.h = 240;
    dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    //curseurLink
    src.x = 0; src.y = 0; src.w = 16; src.h = 21;
    dst.x = 26 + 152 * column; dst.y = dst.y = 197;
    SDL_BlitSurface(curseurLink, &src, gpScreen, &dst);
    
    if (erase) {
        src.x = 0; src.y = 0; src.w = 112; src.h = 72;
        dst.x = 104; dst.y = 84;
        SDL_BlitSurface(imageErase, &src, gpScreen, &dst);
        
        src.h = 16; src.w = 16; src.x = 0; src.y=0;
        dst.x = 112; dst.y = 113 + 20 * line;
        SDL_BlitSurface(curseurFee, &src, gpScreen, &dst);
    }
}
