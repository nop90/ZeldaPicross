/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <sstream>

#include "CaseIcon.h"
#include "StringTokeneizer.h"
#include "Resources.h"

CaseIcon::CaseIcon(int theme) {
    imageTheme = Resources::getInstance()->getMap(theme);
	type = ID_ICON;
}

CaseIcon::~CaseIcon() {
}

void CaseIcon::loadFromString(string line) {
    StringTokeneizer st(line);
    string value;
    for (int i = 0; i < 11; i++) {
        value = st.next();
        istringstream ss(value);
        int v; 
        ss >> v;
        
        switch(i) {
            case 0 : id = v; break;
            case 1 : x = v; break;
            case 2 : y = v; break;
            case 3 : imageId = v; image = Resources::getInstance()->getIcon(v); break;
            case 4 : up = (v > 0); break;
            case 5 : down = (v > 0); break;
            case 6 : left = (v > 0); break;
            case 7 : right = (v > 0); break;
            case 8 : destLvl = v; break;
            case 9 : destCase = v; break;
            case 10 : state = (v > 0) ? DONE : TODO; break;
        }
    }
}

int CaseIcon::getDestLvl() {return destLvl;}
int CaseIcon::getDestCase() {return destCase;}
int CaseIcon::getImageId() {return imageId;}


void CaseIcon::draw(SDL_Surface* gpScreen, bool display, bool map, bool compas) {
    if (display && map && compas); // avoid a warning
    SDL_Rect src;
    SDL_Rect dst;
    src.w = ICON_SIZE; 
    src.h = ICON_SIZE;
    src.x = 0;
    src.y = 0;
    dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y + PATH_CASE_MAP_SIZE; 
    dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x + PATH_CASE_MAP_SIZE;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    int i = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x;
    int j = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y;
    
    if (((state == DONE || map) && up) || forceUp) {
        src.x = 0; src.y = 0; 
        src.w = TOTAL_CASE_MAP_SIZE;
        src.h = PATH_CASE_MAP_SIZE;
        dst.x = i; dst.y = j;
        if (state != DONE && !forceUp) {
            src.x += 20;
        }
        SDL_BlitSurface(imageTheme, &src, gpScreen, &dst);
    }
    if (((state == DONE || map) && down) || forceDown) {
        src.x = 0; src.y = TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE; 
        src.w = TOTAL_CASE_MAP_SIZE;
        src.h = PATH_CASE_MAP_SIZE;
        dst.x = i; dst.y = j + TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE;
        if (state != DONE && !forceDown) {
            src.x += 20;
        }
        SDL_BlitSurface(imageTheme, &src, gpScreen, &dst);
    }
    if (((state == DONE || map) && left) || forceLeft) {
        src.x = 0; src.y = 0;
        src.h = TOTAL_CASE_MAP_SIZE;
        src.w = PATH_CASE_MAP_SIZE;
        dst.x = i; dst.y = j;
        if (state != DONE && !forceLeft) {
            src.x += 20;
        }
        SDL_BlitSurface(imageTheme, &src, gpScreen, &dst);
    }
    if (((state == DONE || map) && right) || forceRight) {
        src.y = 0; src.x = TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE;
        src.h = TOTAL_CASE_MAP_SIZE;
        src.w = PATH_CASE_MAP_SIZE;
        dst.x = i + TOTAL_CASE_MAP_SIZE - PATH_CASE_MAP_SIZE; dst.y = j;
        if (state != DONE && !forceRight) {
            src.x += 20;
        }
        SDL_BlitSurface(imageTheme, &src, gpScreen, &dst);
    }
}
