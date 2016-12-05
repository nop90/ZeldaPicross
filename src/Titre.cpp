/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Titre.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"

Titre Titre::instance=Titre();

Titre::Titre() {
}

Titre::~Titre() {
}

void Titre::init() {
    imageBackground = Resources::getInstance()->getTitreBG();
    imageLogo = Resources::getInstance()->getTitreLogo();
}

void Titre::start() {
    Audio::getInstance()->playMusic(200);
}

Titre* Titre::getInstance() {
    return &instance;
}

void Titre::handleEvent(Event* event) {
    if (event->RETURN) {
        Audio::getInstance()->stopMusic();
        Game::getInstance()->setMode(MENU);
    }
}

void Titre::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.w = 320; src.h = 240;
    dst.x = 0; dst.y = 0;
    SDL_BlitSurface(imageBackground, &src, gpScreen, &dst);
    
    src.x = 0; src.y = 0; src.w = 149; src.h = 105;
    dst.x = 85; dst.y = 67;
    SDL_BlitSurface(imageLogo, &src, gpScreen, &dst);
}
