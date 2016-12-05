/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CASEICON_H__
#define __CASEICON_H__

#include "3ds/SDL_3ds.h"

#include "Common.h"
#include "CaseMap.h"

#define ID_ICON 2

class CaseIcon : public CaseMap {
    public :
        CaseIcon(int thm);
        ~CaseIcon();
        void draw(SDL_Surface* gpScreen, bool display, bool map, bool compas);
        void loadFromString(string line);
        int getDestLvl();
        int getDestCase();
        int getImageId();
    private :
        
        int destLvl;
        int destCase;
        int imageId;
        SDL_Surface* imageTheme;
        
};

#endif  // CaseIcon.h
