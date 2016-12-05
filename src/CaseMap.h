/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CASEMAP_H__
#define __CASEMAP_H__

#include "3ds/SDL_3ds.h"

#include "Common.h"

class CaseMap {
    public :
        CaseMap();
        virtual ~CaseMap();
        virtual void draw(SDL_Surface* gpScreen, bool display, bool map, bool compas) = 0;
        virtual void loadFromString(string line) = 0;
        int getId();
        int getX();
        int getY();
        bool isUp();
        bool isDown();
        bool isLeft();
        bool isRight();
        bool isForceUp();
        bool isForceDown();
        bool isForceLeft();
        bool isForceRight();
        void setForceUp(bool b);
        void setForceDown(bool b);
        void setForceLeft(bool b);
        void setForceRight(bool b);
        CaseMapState getStatus();
        void setStatus(CaseMapState status);
		int type;
    protected :
        
        int id;
        int x;
        int y;
        //int picrossId; // picross id for this case
        
        //int texteId; // text to display once the picross is achieved
        bool up; // if we can go away to the up on this case
        bool down; // if we can go away to the down on this case
        bool left; // if we can go away to the left on this case
        bool right; // if we can go away to the right on this case
        bool forceUp;
        bool forceDown;
        bool forceLeft;
        bool forceRight;
        //int musique;
        //int destLvl;
        //int destCase;
        //int time;
        //int degats;
        //Object required; // object needed to try the picross of this case
        //Object object; // object to win
        
        CaseMapState state; // done or todo
        
        SDL_Surface* image;
};

#endif  // CaseMap.h
