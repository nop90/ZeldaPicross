/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Game.h"
#include "Event.h"
#include "Text.h"
#include "Transition.h"
#include "Cadre.h"
#include "Logo.h"
#include "Titre.h"
#include "Menu.h"
#include "Options.h"
#include "Records.h"
#include "Intro.h"
#include "Ending.h"
#include "Free.h"

Game Game::instance=Game();

Game::Game() : save(0), mode(LOGO), volume(32), volson(32), rRank(-1), 
rQuest(0), rFree(0), rQuestTime(360000), rFreeTime(360000), rToSave(false) {
}

Game::~Game() {
    //delete gpQuest;
    for (int i = 0; i < 3; i++) {
        delete joueurs[i];
    }
}

void Game::init() {
    loadSystem();
    for (int i = 0; i < 3; i++) {
        joueurs[i] = new Joueur(i + 1);
    }
    Logo::getInstance()->init();
    Titre::getInstance()->init();
    Text::getInstance()->init();
    Transition::getInstance()->init();
    Cadre::getInstance()->init();
    Menu::getInstance()->init(joueurs);
    Options::getInstance()->init(volume, volson);
    Records::getInstance()->init();
    Intro::getInstance()->init();
    Ending::getInstance()->init();
    Free::getInstance()->init();
    Quest::getInstance()->init();
}

Game* Game::getInstance() {
    return &instance;
}

void Game::saveSystem() {
    if (!rToSave) {
        return;
    }
    ofstream f("/3ds/ZeldaPicross/save/system.dat",ios::out | ios::binary);
    f.write((char *)&volume,sizeof(int));
    f.write((char *)&volson,sizeof(int));
    f.write((char *)&rRank,sizeof(int));
    f.write((char *)&rQuest,sizeof(int));
    f.write((char *)&rFree,sizeof(int));
    f.write((char *)&rQuestTime,sizeof(int));
    f.write((char *)&rFreeTime,sizeof(int));
    f.close();
    rToSave = false;
}

void Game::loadSystem() {
    ifstream f("/3ds/ZeldaPicross/save/system.dat",ios::in | ios::binary);
    if(!f.is_open()) return;
    f.read((char *)&volume,sizeof(int));
    f.read((char *)&volson,sizeof(int));
    f.read((char *)&rRank,sizeof(int));
    f.read((char *)&rQuest,sizeof(int));
    f.read((char *)&rFree,sizeof(int));
    f.read((char *)&rQuestTime,sizeof(int));
    f.read((char *)&rFreeTime,sizeof(int));
    f.close();
}

void Game::setVolume(int vm) {
    if (volume != vm) {
        rToSave = true;
    }
    volume = vm;
}

void Game::setVolson(int vs) {
    if (volson != vs) {
        rToSave = true;
    }
    volson = vs;
}

void Game::setRank(int r) {
    if (r > rRank) {
        rRank = r;
        rToSave = true;
    }
}

void Game::setQuest(int q) {
    if (q > rQuest) {
        rQuest = q;
        rToSave = true;
    }
}

void Game::setFree(int f) {
    if (f > rFree) {
        rFree = f;
        rToSave = true;
    }
}

void Game::setQuestTime(int qt) {
    if (qt < rQuestTime) {
        rQuestTime = qt;
        rToSave = true;
    }
}

void Game::setFreeTime(int ft) {
    if (ft < rFreeTime) {
        rFreeTime = ft;
        rToSave = true;
    }
}

void Game::setSave(int s) {
    if (mode == MENU) {
        save = s;
    }
}

void Game::setMode(Mode m) {
    if (m == TITLE) {
        Titre::getInstance()->start();
    } else if (m == MENU && mode != OPTIONS) {
        Menu::getInstance()->start();
    } else if (m == OPTIONS) {
        Options::getInstance()->start();
    } else if (m == RECORDS) {
        Records::getInstance()->start(
            rRank, rQuest, rFree, rQuestTime, rFreeTime);
    } else if (m == INTRO) {
        Intro::getInstance()->start();
    } else if (m == ENDING) {
        Ending::getInstance()->start(joueurs[save - 1]);
    } else if (m == QUEST) {
        Quest::getInstance()->start(joueurs[save - 1]);
    } else if (m == FREE) {
        Free::getInstance()->start(joueurs[save - 1]);
    }
    mode = m;
}

void Game::razRecords() {
    rRank = -1;
    rQuest = 0;
    rFree = 0;
    rQuestTime = 360000;
    rFreeTime = 360000;
    rToSave = true;
    saveSystem();
    if (mode == RECORDS) {
        Records::getInstance()->start(
            rRank, rQuest, rFree, rQuestTime, rFreeTime);
    }
}

void Game::handleEvent(Event* event) {
    
    if (event->QUIT && !event->ESCAPE) {
        return;
    }
    
    if (Transition::getInstance()->isToDisplay()) {
        if (event->ESCAPE) event->QUIT = false;
        return;
    }
    
    if (Text::getInstance()->isToDisplay()) {
        if (event->ESCAPE) event->QUIT = false;
        Text::getInstance()->handleEvent(event);
        if (Text::getInstance()->isToDisplay()) {
            return;
        }
        event->RETURN = false;
    }
    
    switch (mode) {
        case LOGO :
            Logo::getInstance()->handleEvent(event);
            break;
        case TITLE :
            Titre::getInstance()->handleEvent(event);
            break;
        case MENU :
            Menu::getInstance()->handleEvent(event);
            break;
        case OPTIONS :
            Options::getInstance()->handleEvent(event);
            break;
        case RECORDS :
            Records::getInstance()->handleEvent(event);
            break;
        case INTRO :
            Intro::getInstance()->handleEvent(event);
            break;
        case ENDING :
            Ending::getInstance()->handleEvent(event);
            break;
        case QUEST :
            Quest::getInstance()->handleEvent(event);
            if (event->QUIT) {
                event->QUIT = false;
                setMode(LOGO);
            }
            break;
        case FREE :
            Free::getInstance()->handleEvent(event);
            if (event->QUIT) {
                event->QUIT = false;
                setMode(LOGO);
            }
            break;
        default : break;
    }
}

void Game::loop() {
    
    if (Transition::getInstance()->isToDisplay()) {
        Transition::getInstance()->loop();
    }
    
    switch (mode) {
        case QUEST :
            Quest::getInstance()->loop();
            break;
        case FREE :
            Free::getInstance()->loop();
            break;
        default : break;
    }
}

void Game::draw(SDL_Surface* gpScreen) {
    switch (mode) {
        case LOGO :
            Logo::getInstance()->draw(gpScreen);
            break;
        case TITLE :
            Titre::getInstance()->draw(gpScreen);
            break;
        case MENU :
            Menu::getInstance()->draw(gpScreen);
            break;
        case OPTIONS :
            Options::getInstance()->draw(gpScreen);
            break;
        case RECORDS :
            Records::getInstance()->draw(gpScreen);
            break;
        case INTRO :
            Intro::getInstance()->draw(gpScreen);
            break;
        case ENDING :
            Ending::getInstance()->draw(gpScreen);
            break;
        case QUEST :
            Quest::getInstance()->draw(gpScreen);
            break;
        case FREE :
            Free::getInstance()->draw(gpScreen);
            break;
        default : break;
    }
    
    if (Text::getInstance()->isToDisplay()) {
        Text::getInstance()->draw(gpScreen);
    }
    
    if (Transition::getInstance()->isToDisplay()) {
        Transition::getInstance()->draw(gpScreen);
    }
}
