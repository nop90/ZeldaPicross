/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Logo.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"

Logo Logo::instance=Logo();

Logo::Logo() {
}

Logo::~Logo() {
}

void Logo::init() {
    image = Resources::getInstance()->getLogo();
}

Logo* Logo::getInstance() {
    return &instance;
}

void Logo::handleEvent(Event* event) {
    if (event->RETURN) {
        Audio::getInstance()->playSound(1);
        Game::getInstance()->setMode(TITLE);
    }
}

void Logo::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0;
    src.y = 0;
    src.w = 320; 
    src.h = 240;
    dst.x = 0;
    dst.y = 0;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}
