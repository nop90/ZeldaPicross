/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Event.h"
#include "Quest.h"
#include "Picross.h"
#include "Resources.h"
#include "Text.h"
#include "Map.h"
#include "Transition.h"
#include "Audio.h"
#include "Cadre.h"
#include "Chrono.h"
#include "Game.h"

Quest Quest::instance=Quest();

Quest::Quest() : mode(WORLD_MAP), nextMode(WORLD_MAP), nextPicross(0), 
nextMap(0), nextCase(0), picrossVictory(false), quit(false), quitPicross(false), 
info(0), picrossTime(0), picrossDegats(0), previousTime(0), nextPreviousTime(0), 
newHeart(false), newObject(0), displayTimeToBeat(false) {
    gpPicross = Picross::getInstance();
    gpMap = new Map();
}

Quest::~Quest() {
    delete gpMap;
}

Quest* Quest::getInstance() {
    return &instance;
}

void Quest::init() {
}

void Quest::start(Joueur* joueur) {
    mode = WORLD_MAP;
    gpJoueur = joueur;
    //gpJoueur->init();
    gpMap->init(gpJoueur->getMap());
    gpMap->refresh(gpJoueur);
    gpPicross->useKeyboard();
    displayTimeToBeat = !gpMap->getPicrossToDo() 
        && !gpJoueur->getRecord(gpMap->getId() + 1) 
        && gpMap->getObjectToWin();
}

void Quest::handleEvent(Event* event) {
    
    if (quit) {
        int text = Text::getInstance()->getId();
        if (text == TEXTS_BEFORE_COMMON + 5) { // save and quit
            gpJoueur->save();
        }
        if (text == TEXTS_BEFORE_COMMON + 6) { // quit but don't save
            gpJoueur->init();
        }
        if (text == TEXTS_BEFORE_COMMON + 5 
        || text == TEXTS_BEFORE_COMMON + 6) {
            Audio::getInstance()->stopMusic();
            event->QUIT = true;
        }
        quit = false;
        return;
    } else if (quitPicross) {
        int text = Text::getInstance()->getId();
        if (text == TEXTS_BEFORE_COMMON + 10) { // return
            nextMode = WORLD_MAP;
           // Chrono::getInstance()->stop();
            int nbPicross = gpMap->getNbPicross(gpJoueur->getX(), gpJoueur->getY());
            if (nbPicross > 1 && !gpJoueur->getQuestTime(nextPicross)) {
                int start = gpMap->getPicross(gpJoueur->getX(), gpJoueur->getY());
                for (int i = 0; i < nbPicross; i++) {
//                    gpJoueur->setPicross(start + i, 0);
                    gpJoueur->setQuestTime(start + i, 0);
                }
            }
            Transition::getInstance()->begin();
        } else {
            gpPicross->setPause(false);
            Chrono::getInstance()->continu();
        }
        quitPicross = false;
        return;
    }
    
    //catch Escape event
    if (event->ESCAPE) {
        switch (mode) {
            case WORLD_MAP :
                Text::getInstance()->setText(TEXTS_BEFORE_COMMON + 4, 
                    8, 152, 304, 80, 3);
                quit = true;
                break;
            case RESOLVE_PICROSS :
                if (gpJoueur->getVie() == 0) {
                    break;
                }
                Text::getInstance()->setText(TEXTS_BEFORE_COMMON + 9, 
                    8, 152, 304, 80, 3);
                quitPicross = true;
                gpPicross->setPause(true);
                Chrono::getInstance()->pause();
                break;
            default : break;
        }
        event->QUIT = false;
        return;
    }
    
    switch (mode) {
        case WORLD_MAP :
            handleMoveOnMapEvent(event);
            break;
        case RESOLVE_PICROSS :
            if (event->MOUSE_ON_OFF && !gpJoueur->hasObject(SWORD)) {
                event->MOUSE_ON_OFF = false;
            }
            if (event->HYPOTHESE && !gpJoueur->hasObject(BOOK)) {
                event->HYPOTHESE = false;
            }
            gpPicross->handleEvent(event);
            if (gpPicross->isDefeat() && event->RETURN) {
                nextMode = WORLD_MAP;
                Transition::getInstance()->begin();
                break;
            }
            if (event->ERROR) {
                Audio::getInstance()->playSound(15);
                gpJoueur->removeVie(picrossDegats);
                if (!gpJoueur->getVie()) {
                    gpPicross->setDefeat(true);
                    Audio::getInstance()->playMusic(220);
                    Chrono::getInstance()->stop();
                    int nbPicross = gpMap->getNbPicross(gpJoueur->getX(), gpJoueur->getY());
                    if (nbPicross > 1 && !gpJoueur->getQuestTime(nextPicross)) {
                        int start = gpMap->getPicross(gpJoueur->getX(), gpJoueur->getY());
                        for (int i = 0; i < nbPicross; i++) {
                            gpJoueur->setQuestTime(start + i, 0);
                        }
                    }
                }
            }
            break;
        default : break;
    }
}

void Quest::handleMoveOnMapEvent(Event* event) {
    
    int x = gpJoueur->getX();
    //int oldx = x;
    int y = gpJoueur->getY();
    //int oldy = y;
    
    if (event->RETURN) {
        int p = gpMap->getPicross(x, y);
        if (p > 0) {
            if (!gpMap->hasObject(x, y)) {
                return;
            }
            picrossVictory = false;
            nextMode = RESOLVE_PICROSS;
            nextPicross = p;
            Transition::getInstance()->begin();
        } else { // icon
            nextMap = gpMap->getDestLvl(x, y);
            nextCase = gpMap->getDestCase(x, y);
            nextMode = WORLD_MAP;
            gpMap->playIconSound(x, y);
            Transition::getInstance()->begin();
        }
        return;
    }
    
    
    
    if (event->UP) {
        if (gpMap->isUp(x,y)) {
            gpJoueur->moveY(-1);
        }
    } else if (event->DOWN) {
        if (gpMap->isDown(x,y)) {
            gpJoueur->moveY(1);
        }
    } else if (event->LEFT) {
        if (gpMap->isLeft(x,y)) {
            gpJoueur->moveX(-1);
        }
    } else if (event->RIGHT) {
        if (gpMap->isRight(x,y)) {
            gpJoueur->moveX(1);
        }
    }
    
    //x = gpJoueur->getX();
    //y = gpJoueur->getY();
    
    //if (x != oldx || y!= oldy) {
        // if Link moved ...
    //}
}

void Quest::loop() {
    
    if (gpJoueur->getTutoriel() == 0) {
        Text::getInstance()->setText(TEXTS_BEFORE_TUTO + 2, 16, 152, 288, 
            80, 2, false);
        gpJoueur->setTutoriel(1);
        info = -1;
        return;
    }
    
    Transition* gpTransition = Transition::getInstance();
    if (gpTransition->isToDisplay()) {
        if (gpTransition->getStep() == WAIT) {
            QuestMode oldMode = mode;
            mode = nextMode;
            switch (mode) {
                case WORLD_MAP :
                    if (gpJoueur->getTutoriel() < 7) {
                        if (gpJoueur->getTutoriel() == 0) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 2, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(1);
                            info = -1;
                        } else if (nextPicross == 1) {
                            if (gpJoueur->getTutoriel() == 2 
                            && gpJoueur->getQuestTime(1)) {
                                Text::getInstance()->setText(
                                    TEXTS_BEFORE_TUTO + 4, 16, 152, 288, 
                                    80, 2, false);
                                gpJoueur->setTutoriel(3);
                                info = -1;
                            }
                        }
                    }
                    if (newHeart) {
                        newHeart = false;
                        gpJoueur->setVieMax(gpJoueur->getVieMax()+1);
                        info = (int)HEART;
                    } else if (newObject) {
                        info = newObject;
                        gpJoueur->setObject((Object)newObject);
                        newObject = 0;
                    }
                    if (oldMode == WORLD_MAP) {
                        gpMap->init(nextMap);
                        gpMap->updateCoord(gpJoueur, nextCase);
                        gpJoueur->setMap(nextMap);
                    }
                    Chrono::getInstance()->raz();
                    previousTime = 0;
                    nextPreviousTime = 0;
                    gpJoueur->setVie(gpJoueur->getVieMax());
                    gpMap->refresh(gpJoueur);
                    displayTimeToBeat = !gpMap->getPicrossToDo() 
                        && !gpJoueur->getRecord(gpMap->getId() + 1) 
                        && gpMap->getObjectToWin();
                    break;
                case RESOLVE_PICROSS : 
                    if (gpJoueur->getTutoriel() < 7) {
                        if (gpJoueur->getTutoriel() == 1 && nextPicross == 1) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 3, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(2);
                            info = -1;
                        }
                        if (gpJoueur->getTutoriel() == 3 && nextPicross == 2) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 5, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(4);
                            info = -1;
                        }
                        if (gpJoueur->getTutoriel() == 4 && nextPicross == 3) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 6, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(5);
                            info = -1;
                        }
                        if (gpJoueur->getTutoriel() == 5 && nextPicross == 4) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 7, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(6);
                            info = -1;
                        }
                        if (gpJoueur->getTutoriel() == 6 && nextPicross == 5) {
                            Text::getInstance()->setText(
                                        TEXTS_BEFORE_TUTO + 8, 16, 152, 288, 
                                        80, 2, false);
                            gpJoueur->setTutoriel(7);
                            info = -1;
                        }
                    }
                    previousTime += nextPreviousTime;
                    picrossTime = gpMap->getTime(gpJoueur->getX(), gpJoueur->getY());
                    if (gpJoueur->hasObject(BOOTS)) {
                        picrossTime *= 2;
                    }
                    picrossDegats = gpMap->getDegats(gpJoueur->getX(), gpJoueur->getY());
                    background = Resources::getInstance()->getPicrossBG(
                        gpMap->getPicrossBackground());
                    gpPicross->init(Resources::getInstance()->getPicross(nextPicross), 
                        Resources::getInstance()->getTheme(gpMap->getPicrossTheme()));
                    Audio::getInstance()->playMusic(gpMap->getPicrossMusic(
                        gpJoueur->getX(), gpJoueur->getY()));
                    break;
                default : break;
            }
            gpTransition->next();
        }
        if (gpTransition->isFinalRound() && mode == RESOLVE_PICROSS) {
            Chrono::getInstance()->start();
        }
        return;
    }
    
    if (info == -1 && !Text::getInstance()->isToDisplay()) {
        info = 0;
        if (displayTimeToBeat && gpJoueur->getRecord(gpMap->getId() + 1)) {
            displayTimeToBeat = false;
        }
    }
    
    switch (mode) {
        case WORLD_MAP :
            if (!info && !gpMap->getPicrossToDo() 
            && gpMap->getObjectToWin() != NOTHING) {
                if (!gpJoueur->getTutoRecords()) {
                    
                    Text::getInstance()->setText(TEXTS_BEFORE_TUTO + 1, 
                        16, 152, 288, 80, 2, false);
                    info = -1;
                    gpJoueur->setTutoRecords();
                    
                } else if (!gpJoueur->getRecord(gpMap->getId() + 1) 
                    && gpMap->getCumulatedTime() < gpMap->getTimeToBeat()) {
                
                    int object = gpMap->getObjectToWin();
                    info = (int)object;
                    if (object == HEART) {
                        gpJoueur->setVieMax(gpJoueur->getVieMax() + 1);
                        gpJoueur->setVie(gpJoueur->getVieMax());
                    } else {
                        gpJoueur->setObject((Object)object);
                    }
                    gpJoueur->setRecord(gpMap->getId() + 1);
                }
            }
            if (info > 0 && !Text::getInstance()->isToDisplay()) {
                Text::getInstance()->setText(TEXTS_BEFORE_FOUND + info, 
                    16, 152, 288, 80, 2, false);
                Audio::getInstance()->playSound(10);
                info = -1;
            }
            break;
        case RESOLVE_PICROSS :
            if (gpPicross->isVictory() && gpJoueur->getVie()) {
                if (picrossVictory) {
                    if (!Text::getInstance()->isToDisplay()) {
                        if ((nextPicross + 1) < 
                        (gpMap->getPicross(gpJoueur->getX(), gpJoueur->getY()) 
                        + gpMap->getNbPicross(gpJoueur->getX(), gpJoueur->getY()))) {
                            picrossVictory = false;
                            nextMode = RESOLVE_PICROSS;
                            nextPicross++;
                        } else {
                            nextMode = WORLD_MAP;
                        }
                        if (nextPicross == 200) { // quest finished
                            gpJoueur->save();
                            Game::getInstance()->setMode(ENDING);
                        } else {
                            Transition::getInstance()->begin();
                        }
                    }
                } else {
                    Object object = gpMap->getObject(gpJoueur->getX(), gpJoueur->getY());
                    if (nextPicross + 1 == 
                        gpMap->getPicross(gpJoueur->getX(), gpJoueur->getY()) 
                        + gpMap->getNbPicross(gpJoueur->getX(), gpJoueur->getY()) 
                        && object != NOTHING && !gpJoueur->getQuestTime(nextPicross)) {
                        if (object == HEART) {
                            newHeart = true;
                        } else {
                            newObject = (int)object;
                        }
                    }
//                    gpJoueur->setPicross(nextPicross);
                    if (gpMap->getRequired(
                        gpJoueur->getX(), gpJoueur->getY()) == KEYS
                        && gpJoueur->getQuestTime(nextPicross)  == 0) {
                        gpJoueur->useKey();
                    }
                    Text::getInstance()->setText(
                        TEXTS_BEFORE_PICROSS + nextPicross, 104, 8, 208, 32, 2);
                    Audio::getInstance()->playSound(4);
                    picrossVictory = true;
                    Chrono::getInstance()->stop();
                    nextPreviousTime = Chrono::getInstance()->getTime();
                    gpJoueur->setQuestTime(nextPicross, 
                        Chrono::getInstance()->getTime());
                }
            }
            break;
        default : break;
    }
}

void Quest::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    int width;
    string text;
    Text* gpText = Text::getInstance();
    ostringstream os;
    int i;
    int x = gpJoueur->getX();
    int y = gpJoueur->getY();
    int time;
    
    switch (mode) {
        case WORLD_MAP :
            gpMap->draw(gpScreen, 
                        (gpJoueur->hasObject(MAP) && gpMap->getId() > 5), 
                        (gpJoueur->hasObject(COMPASS) && gpMap->getId() > 5));
            Cadre::getInstance()->drawCadre(gpScreen, 224, 0, 96, 48);
            gpJoueur->draw(gpScreen);
            gpJoueur->drawLife(gpScreen,16*14+8,8);
            gpJoueur->drawObjects(gpScreen,16*14+8,24);
            
            // map name and optional time
            text = gpText->getText(gpMap->getId() + 1);
            if (!gpMap->getPicrossToDo()) {
                text += (" - " + gpText->getTime(gpMap->getCumulatedTime()));
            }
            width = 16 + text.length() * 6 + (16 - (text.length() * 6)%16)%16;
            Cadre::getInstance()->drawCadre(gpScreen, 0, 0, width, 32);
            gpText->affiche(gpScreen, text, 8, 8);
            
            // optional object to win
            /*if (!gpMap->getPicrossToDo() 
                && !gpJoueur->getRecord(gpMap->getId()+1)
                && gpMap->getObjectToWin()) {*/
            if (displayTimeToBeat) {
                Cadre::getInstance()->drawCadre(gpScreen, 0, 34, 80, 32, 1);
                gpJoueur->drawObject(gpScreen, 8, 46, gpMap->getObjectToWin());
                gpText->affiche(gpScreen, 
                    gpText->getTime(gpMap->getTimeToBeat()), 20, 42);
            }
            
            
            // info
            Cadre::getInstance()->drawCadre(gpScreen, 0, 176, 320, 64);
            gpText->affiche(gpScreen, gpMap->getText(x, y), 8, 184);
            i = gpMap->getDegats(x, y);
            if (i > 0) {
                os << i;
                text = gpText->getText(TEXTS_BEFORE_COMMON + 2) + os.str();
                gpText->affiche(gpScreen, text, 8, 200);
                int maxTime = gpMap->getTime(x, y);
                if (gpJoueur->hasObject(BOOTS)) {
                    maxTime *= 2;
                }
                text = gpText->getText(TEXTS_BEFORE_COMMON + 3) 
                    + gpText->getTime(maxTime);
                gpText->affiche(gpScreen, text, 8, 216);
                
                time = 0;
                for (int k = 0; k < gpMap->getNbPicross(x, y); k++) {
                    time += gpJoueur->getQuestTime(gpMap->getPicross(x, y) + k);
                }
                if (time > 0) {
                    text = gpText->getText(TEXTS_BEFORE_COMMON + 7)
                        + gpText->getTime(time);
                    gpText->affiche(gpScreen, text, 160, 216);
                }
            }
            
            // total time
            Cadre::getInstance()->drawCadre(gpScreen, 0, 142, 64, 32, 1);
            gpText->affiche(gpScreen, gpText->getTime(gpJoueur->getTotalQuestTime()),6,150);
            
            break;
        case RESOLVE_PICROSS :
            src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
            SDL_BlitSurface(background, &src, gpScreen, &dst);
            gpPicross->draw(gpScreen);
            gpJoueur->drawLife(gpScreen,16,16);
            
            if (gpPicross->isVictory() || gpPicross->isDefeat()) {
                time = Chrono::getInstance()->getTime() + previousTime;
            }
            else {
                time = Chrono::getInstance()->getCurrentTime() + previousTime;
            }
            
            time = picrossTime - time;

            
            
            if (time <= 0 && gpJoueur->getVie()) {
                Audio::getInstance()->playSound(15);
                gpJoueur->removeVie(picrossDegats);
                if (!gpJoueur->getVie()) {
                    gpPicross->setDefeat(true);
                    Audio::getInstance()->playMusic(220);
                    Chrono::getInstance()->stop();
                    int nbPicross = gpMap->getNbPicross(gpJoueur->getX(), gpJoueur->getY());
                    if (nbPicross > 1 && !gpJoueur->getQuestTime(nextPicross)) {
                        int start = gpMap->getPicross(gpJoueur->getX(), gpJoueur->getY());
                        for (int i = 0; i < nbPicross; i++) {
//                            gpJoueur->setPicross(start + i);
                            gpJoueur->setQuestTime(start + i, 0);
                        }
                    }
                } else {
                    int newDelay = gpJoueur->hasObject(BOOTS)?60:30;
                    picrossTime += newDelay;
                    time += newDelay;
                }
            }
            
            gpText->affiche(gpScreen, gpText->getTime(time), 16, 32);
            break;
        default : break;
    }
}
