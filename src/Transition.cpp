/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Transition.h"

Transition Transition::instance=Transition();

Transition::Transition() : mode(NONE), step(END), anim(0), animMax(320), 
toDisplay(false), x(160), y(120), image(0) {
}

Transition::~Transition() {
    SDL_FreeSurface(image);
}

Transition* Transition::getInstance() {
    return &instance;
}

void Transition::init(TransitionMode m) {
    mode = m;
    SDL_FreeSurface(image);
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    switch (mode) {
        case CIRCLE : animMax = 320; break;
        default : break;
    }
}

void Transition::begin(int i, int j) {
    x = i;
    y = j;
    step = IN;
    anim = 0;
    toDisplay = true;
}

void Transition::next(int i, int j) {
    x = i;
    y = j;
    step = OUT;
    anim = 0;
}

bool Transition::isToDisplay() {return toDisplay;}

TransitionStep Transition::getStep() {return step;}

void Transition::loop() {
    int rayon = 0;
    switch (mode) {
        case CIRCLE :
            if (anim == animMax) {
                if (step == IN) {
                    SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 0, 0, 0));
                    step = WAIT;
                }
                else if (step == OUT) {
                    step = END;
                    toDisplay = false;
                }
                break;
            }
            anim+=10;
            SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 0, 0, 0));
            if (step == IN) {
                rayon = 320 - anim;
            }
            else if (step == OUT) {
                rayon = anim;
            }
            filledEllipseRGBA(image, x, y, rayon, rayon*2/3, 0, 0, 255, 255);
            SDL_SetColorKey(image,SDL_SRCCOLORKEY, SDL_MapRGB(image->format,0,0,255));
            break;
        default : break;
    }
}

bool Transition::isFinalRound() {
    switch (mode) {
        case CIRCLE : 
            return (anim + 10 == animMax && step == OUT);
        default : return false;
    }
}

void Transition::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    switch (mode) {
        case CIRCLE :
            src.w = 320; src.h = 240; src.x = 0; src.y = 0; 
            dst.y = 0; dst.x = 0;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            break;
        default : break;
    }
}
