/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __JOUEUR_H__
#define __JOUEUR_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"
#include "Picross.h"
#include "Joueur.h"

class Joueur {
    public :
        Joueur(int id);
        ~Joueur();
        void draw(SDL_Surface* gpScreen);
        void drawLife(SDL_Surface* gpScreen, int x, int y);
        void drawObjects(SDL_Surface* gpScreen, int x, int y);
        void drawObject(SDL_Surface* gpScreen, int i, int j, int obj);
        void init();
        void save();
        void load();
        void moveX(int m);
        void moveY(int m);
        int getX();
        int getY();
        int getMap();
        int getQuestTime(int i);
        int getFreeTime(int i);
//        int getPicross(int i);
//        void setPicross(int i, int v = 1);
        int getVie();
        int getVieMax();
        void setX(int i);
        void setY(int i);
        void setVie(int i);
        void setVieMax(int i);
        void removeVie(int degats);
        void setMap(int i);
        void setQuestTime(int i, int t);
        void setFreeTime(int i, int t);
        bool hasObject(Object obj);
        void setObject(Object obj);
        void useKey();
        int getTotalQuestTime();
        int getTotalFreeTime();
        int getRecord(int i);
        void setRecord(int i);
        int getTutoRecords();
        void setTutoRecords();
        int getTutoriel();
        void setTutoriel(int i);
        bool isLoaded();
        string getRank();
        int getQuest();
        int getFree();
        int getPrefBg();
        int getPrefTh();
        int getPrefMs();
        void setPrefBg(int i);
        void setPrefTh(int i);
        void setPrefMs(int i);
    private :
        void computeTotalQuestTime();
        void computeTotalFreeTime();
        int getRankNumber();
        
        int id;  // save number
        bool loaded;
        
        int x;    // x coordinate on the map
        int y;    // y coordinate on the map
        int map;  // current map
        int vie;
        int viemax;
        int tutoRecords;        // 1 if tuto for record is played
        int tutoriel;           // + 1 after each tutoriel readed
        int objects[TOTAL_OBJ];
//        int picross[NB_PICROSS+1];
        int questTime[NB_PICROSS+1];
        int freeTime[NB_PICROSS+1];
        int records[NB_MAPS];   // 1 if record beated for the map, 0 if not yet
        SDL_Surface* image;
        SDL_Surface* imageStatus;
        int totalQuestTime;
        int totalFreeTime;
        int quest;
        int free;
        int prefBg;
        int prefTh;
        int prefMs;
};

#endif  // Joueur.h
