/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GAME_H__
#define __GAME_H__

#include "3ds/SDL_3ds.h"

#include "Event.h"
#include "Common.h"
#include "Quest.h"
#include "Joueur.h"

class Game {
    public :
        static Game* getInstance();
        
        void init();
        void handleEvent(Event* event);
        void loop();
        void draw(SDL_Surface* gpScreen);
        
        void saveSystem();
        void setVolume(int vm);
        void setVolson(int vs);
        void setRank(int r);
        void setQuest(int q);
        void setFree(int f);
        void setQuestTime(int qt);
        void setFreeTime(int ft);
        void setSave(int s);
        void setMode(Mode m);
        void razRecords();
    private :
        static Game instance;
        
        Game();
        ~Game();
        
        void loadSystem();
        
        int save;
        Mode mode;
        int volume;
        int volson;
        int rRank;
        int rQuest;
        int rFree;
        int rQuestTime;
        int rFreeTime;
        bool rToSave;
        Joueur* joueurs[3];
};

#endif  // Game.h
