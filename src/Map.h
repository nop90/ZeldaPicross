/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __MAP_H__
#define __MAP_H__

#include "3ds/SDL_3ds.h"

#include "Common.h"
#include "CaseMap.h"
#include "Joueur.h"

class Map {
    public :
        Map();
        ~Map();
        void init(int id);
        void draw(SDL_Surface* gpScreen, bool hasMap, bool hasCompas);
        bool isUp(int x, int y);
        bool isDown(int x, int y);
        bool isLeft(int x, int y);
        bool isRight(int x, int y);
        int getPicross(int x, int y);
        int getNbPicross(int x, int y);
        string getText(int x, int y);
        void refresh(Joueur* gpJoueur);
        void updateCoord(Joueur* gpJoueur, int nextCase);
        int getPicrossTheme();
        int getPicrossBackground();
        int getPicrossMusic(int x, int y);
        int getDegats(int x, int y);
        int getTime(int x, int y);
        int getId();
        Object getRequired(int x, int y);
        Object getObject(int x, int y);
        bool hasObject(int x, int y);
        int getDestLvl(int x, int y);
        int getDestCase(int x, int y);
        int getPicrossToDo();
        int getCumulatedTime();
        int getTimeToBeat();
        int getObjectToWin();
        void playIconSound(int i, int j);
    private :
        void loadFromString(string line);
        bool isToDisplay(int i, int j);
        
        int mapId;
        string name;
        int musique;
        int background;
        int theme;
        int picrossBackground;
        int picrossTheme;
        int timeToBeat;
        int objectToWin;
        int picrossToDo;
        int cumulatedTime;
        int xJoueur; // case where the player is should always be displayed
        int yJoueur;
        
        CaseMap* map[MAX_CASE_X][MAX_CASE_Y];
        bool toDisplay[MAX_CASE_X][MAX_CASE_Y];
        
        SDL_Surface* image; // background image
};

#endif  // Map.h
