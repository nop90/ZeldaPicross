/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Chrono.h"

Chrono Chrono::instance=Chrono();

Chrono::Chrono() : startTime(0), previous(0), time(0) {
}

Chrono::~Chrono() {
}

Chrono* Chrono::getInstance() {
    return &instance;
}

void Chrono::start() {
    startTime = SDL_GetTicks();
}

void Chrono::stop() {
    time = previous + (SDL_GetTicks() - startTime);
    startTime = 0;
    previous = 0;
}

void Chrono::pause() {
    stop();
    previous = time;
}

void Chrono::continu() {
    time = 0;
    start();
}

void Chrono::raz() {
    time = 0;
    previous = 0;
}

int Chrono::getTime() {return time / 1000;}

int Chrono::getCurrentTime() {
    if (startTime == 0) return previous / 1000;
    return (previous + (SDL_GetTicks() - startTime)) / 1000;
}

