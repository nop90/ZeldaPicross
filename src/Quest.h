/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __QUEST_H__
#define __QUEST_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"
#include "Picross.h"
#include "Joueur.h"
#include "Map.h"

class Quest {
    public :
        static Quest* getInstance();
        
        
        void init();
        void start(Joueur* joueur);
        void handleEvent(Event* event);
        void loop();
        void draw(SDL_Surface* gpScreen);
    private :
        static Quest instance;
        
        Quest();
        ~Quest();
        
        void handleMoveOnMapEvent(Event* event);
        
        QuestMode mode;
        QuestMode nextMode;
        int nextPicross;
        int nextMap;
        int nextCase;
        bool picrossVictory;
        bool quit;
        bool quitPicross;
        int info;
        Joueur* gpJoueur;
        Map* gpMap;
        Picross* gpPicross;
        
        SDL_Surface* background;
        
        int picrossTime;
        int picrossDegats;
        int previousTime;
        int nextPreviousTime;
        bool newHeart;
        int newObject;
        bool displayTimeToBeat;
};

#endif  // Quest.h
