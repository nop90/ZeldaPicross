/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Intro.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"

Intro Intro::instance=Intro();

Intro::Intro() {
}

Intro::~Intro() {
}

Intro* Intro::getInstance() {
    return &instance;
}

void Intro::init() {
    for (int i = 0; i < 3; i++) {
        imageStep[i] = Resources::getInstance()->getStep(i);
    }
    imageFond1 = Resources::getInstance()->getIntroFond1();
    imageFond2 = Resources::getInstance()->getIntroFond2();
}

void Intro::start() {
    Audio::getInstance()->playMusic(210);
    anim = 0;
    lastAnimTime = SDL_GetTicks();
    
    Text::getInstance()->setText(TEXTS_BEFORE_INTRO + 1, 32, 158, 256, 64, 0);
    Text::getInstance()->setNumber(3);
}


void Intro::handleEvent(Event* event) {
    if (event); // avoid warning
    Game::getInstance()->setMode(QUEST);
}

void Intro::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    if(SDL_GetTicks() > lastAnimTime + 40) {
        lastAnimTime = SDL_GetTicks();
        anim++;
        if (anim > 31) anim = 0;
    }
                
    src.h = 16; src.w = 16;
    for (int j=0; j<240; j+=16) 
        for (int i=0; i<320; i+=16) {
            src.x = 16-anim%16;src.y=anim%16; dst.x = i; dst.y = j;
            if (i>16 && i<288 && j>144 && j<224) src.x+=32;
            SDL_BlitSurface(imageFond2, &src, gpScreen, &dst);
        }
                
    src.h = 32; src.w = 32;
    for (int j=0; j<240; j+=32) 
        for (int i=0; i<320; i+=32) {
            src.x = anim;src.y=anim; dst.x = i; dst.y = j;
            if (i>16 && i<288 && j>144 && j<224) src.x+=64;
            SDL_BlitSurface(imageFond1, &src, gpScreen, &dst);
        }
                
    src.h = 111; src.w = 320;
    src.x = 0; src.y=0; dst.x = 0; dst.y = 17;
    SDL_BlitSurface(imageStep[
        Text::getInstance()->getId() - TEXTS_BEFORE_INTRO - 1], 
        &src, gpScreen, &dst);
}
