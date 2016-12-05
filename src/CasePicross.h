/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CASEPICROSS_H__
#define __CASEPICROSS_H__

#include "3ds/SDL_3ds.h"

#include "Common.h"
#include "CaseMap.h"

#define ID_PICROSS 1

class CasePicross : public CaseMap {
    public :
        CasePicross(int thm);
        ~CasePicross();
        void draw(SDL_Surface* gpScreen, bool display, bool map, bool compas);
        void loadFromString(string line);
        int getPicrossId();
        int getMusic();
        int getDegats();
        int getTime();
        Object getRequired();
        Object getObject();
        int getNbPicross();
        int getBossId();
        void setHasObject(bool b);
        bool isHasObject();
    private :
        void displayChest(SDL_Surface* gpScreen);
        
        /*int id;
        int x;
        int y;*/
        int picrossId; // picross id for this case
        
        int texteId; // text to display once the picross is achieved
        /*bool up; // if we can go away to the up on this case
        bool down; // if we can go away to the down on this case
        bool left; // if we can go away to the left on this case
        bool right; // if we can go away to the right on this case*/
        int musique;
        //int destLvl;
        //int destCase;
        int time;
        int degats;
        Object required; // object needed to try the picross of this case
        Object object; // object to win
        int nbPicross; // more of one => boss
        int bossId;
        bool hasObject;
        
        //CaseMapState state; // done or todo
        
        SDL_Surface* imageIcon;
        SDL_Surface* imageBoss;
};

#endif  // CasePicross.h
