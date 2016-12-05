/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <sstream>

#include "CasePicross.h"
#include "StringTokeneizer.h"
#include "Resources.h"

CasePicross::CasePicross(int theme) : hasObject(true) {
    image = Resources::getInstance()->getMap(theme);
    imageIcon = Resources::getInstance()->getStatus();
    imageBoss = Resources::getInstance()->getBoss();
	type = ID_PICROSS;
}

CasePicross::~CasePicross() {
}

void CasePicross::loadFromString(string line) {
    StringTokeneizer st(line);
    string value;
    for (int i = 0; i < 15; i++) {
        value = st.next();
        istringstream ss(value);
        int v; 
        ss >> v;
        
        switch(i) {
            case 0 : id = v; break;
            case 1 : x = v; break;
            case 2 : y = v; break;
            case 3 : picrossId = v; break;
            case 4 : up = (v > 0); break;
            case 5 : down = (v > 0); break;
            case 6 : left = (v > 0); break;
            case 7 : right = (v > 0); break;
            case 8 : musique = v; break;
            case 9 : time = v; break;
            case 10 : degats = v; break;
            case 11 : required = (Object)v; break;
            case 12 : object = (Object)v; break;
            case 13 : nbPicross = v; break;
            case 14 : bossId = v; break;
        }
    }
}

int CasePicross::getPicrossId() {return picrossId;}
int CasePicross::getMusic() {return musique;}
int CasePicross::getDegats() {return hasObject ? degats : 0;}
int CasePicross::getTime() {return time;}
Object CasePicross::getRequired() {return required;}
Object CasePicross::getObject() {return object;}
int CasePicross::getNbPicross() {return nbPicross;}
int CasePicross::getBossId() {return bossId;}
bool CasePicross::isHasObject() {return hasObject;}

void CasePicross::setHasObject(bool b) {hasObject = b;}

void CasePicross::draw(SDL_Surface* gpScreen, bool display, bool map, bool compas) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = MAIN_CASE_MAP_SIZE; 
    src.h = MAIN_CASE_MAP_SIZE;
    src.x = PATH_CASE_MAP_SIZE;
    src.y = PATH_CASE_MAP_SIZE;
    dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y + PATH_CASE_MAP_SIZE; 
    dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x + PATH_CASE_MAP_SIZE;
    
    int i = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x;
    int j = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y;
    
    if (state == DONE) {
        if (up) {src.y = 0; src.h += PATH_CASE_MAP_SIZE; dst.y -= PATH_CASE_MAP_SIZE;}
        if (down) {src.h += PATH_CASE_MAP_SIZE;}
        if (left) {src.x = 0; src.w += PATH_CASE_MAP_SIZE; dst.x -= PATH_CASE_MAP_SIZE;}
        if (right) {src.w += PATH_CASE_MAP_SIZE;}
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    } else {
        if (up && map) {src.y = 0; src.h += PATH_CASE_MAP_SIZE; dst.y -= PATH_CASE_MAP_SIZE;}
        if (down && map) {src.h += PATH_CASE_MAP_SIZE;}
        if (left && map) {src.x = 0; src.w += PATH_CASE_MAP_SIZE; dst.x -= PATH_CASE_MAP_SIZE;}
        if (right && map) {src.w += PATH_CASE_MAP_SIZE;}
        src.x += TOTAL_CASE_MAP_SIZE;
        if (required != NOTHING && display) {
            src.x += TOTAL_CASE_MAP_SIZE;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            if (object && compas) displayChest(gpScreen);
            switch (required) {
                case SWORD : src.x=0; src.y=0; break;
                case BOMBS : src.x=8; src.y=0; break;
                case POWER_GLOVES : src.x=16; src.y=0; break;
                case FLIPPERS : src.x=0; src.y=8; break;
                case HOOKSHOT : src.x=8; src.y=8; break;
                case BOW : src.x=16; src.y=8; break;
                case HAMMER : src.x=24; src.y=8; break;
                case KEYS : src.x=24; src.y=0; break;
                case BOSS_KEY : src.x=24; src.y=16; break;
                default : return;
            }
            src.w = 8; src.h = 8;
            dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x + PATH_CASE_MAP_SIZE + 4;
            dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y + PATH_CASE_MAP_SIZE + 4; 
            SDL_BlitSurface(imageIcon, &src, gpScreen, &dst);
            src.w = 12; src.h = 12;
            src.x = 32; src.y = hasObject ? 0 : 12;
            dst.x -= 2; dst.y -= 2;
            SDL_BlitSurface(imageIcon, &src, gpScreen, &dst);
        } else if (nbPicross > 1 && (display || compas)) {
            src.x += TOTAL_CASE_MAP_SIZE;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            src.x = 0; src.y = 0; src.w = 8; src.h = 8;
            dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x + PATH_CASE_MAP_SIZE + 4;
            dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y + PATH_CASE_MAP_SIZE + 4; 
            SDL_BlitSurface(imageBoss, &src, gpScreen, &dst);
        } else if ((!display /*&& map*/) || (object && compas)) {
            src.x += TOTAL_CASE_MAP_SIZE;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            if (object && compas) displayChest(gpScreen);
        } else {
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            if (object && compas && (map || display)) displayChest(gpScreen);
        }
        
        if (forceUp) {
            src.x = 0; src.y = 0; 
            src.w = TOTAL_CASE_MAP_SIZE;
            src.h = PATH_CASE_MAP_SIZE;
            dst.x = i; dst.y = j;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
        }
        if (forceDown) {
            src.x = 0; src.y = TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE; 
            src.w = TOTAL_CASE_MAP_SIZE;
            src.h = PATH_CASE_MAP_SIZE;
            dst.x = i; dst.y = j + TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
        }
        if (forceLeft) {
            src.x = 0; src.y = 0;
            src.h = TOTAL_CASE_MAP_SIZE;
            src.w = PATH_CASE_MAP_SIZE;
            dst.x = i; dst.y = j;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
        }
        if (forceRight) {
            src.y = 0; src.x = TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE;
            src.h = TOTAL_CASE_MAP_SIZE;
            src.w = PATH_CASE_MAP_SIZE;
            dst.x = i + TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE; dst.y = j;
            SDL_BlitSurface(image, &src, gpScreen, &dst);
        }
    }
    
}

void CasePicross::displayChest(SDL_Surface* gpScreen) {
    SDL_Rect src; SDL_Rect dst;
    src.w = 8; src.h = 8; src.x = 8; src.y = 0;
    dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y 
        + PATH_CASE_MAP_SIZE + (MAIN_CASE_MAP_SIZE - 8) / 2; 
    dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x 
        + PATH_CASE_MAP_SIZE + (MAIN_CASE_MAP_SIZE - 8) / 2; 
    
    SDL_BlitSurface(imageBoss, &src, gpScreen, &dst);
}
