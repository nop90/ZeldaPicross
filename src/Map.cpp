/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Map.h"
#include "CaseMap.h"
#include "CasePicross.h"
#include "CaseIcon.h"
#include "StringTokeneizer.h"
#include "Text.h"
#include "Resources.h"
#include "Audio.h"
#include "Cadre.h"

Map::Map() : xJoueur(0), yJoueur(0) {
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            map[i][j] = 0;
            toDisplay[i][j] = false;
        }
    }
}

Map::~Map() {
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            delete map[i][j];
        }
    }
}

void Map::init(int id) {
    mapId = id;
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            delete map[i][j];
            map[i][j] = 0;
            toDisplay[i][j] = false;
        }
    }
    ostringstream os;
    os << id;
    ifstream file(("romfs:/levels/map"+os.str()+".pmp").c_str());
    
    string line;
    getline(file, line);
    loadFromString(line);
    
    int i = 0;
    while (getline(file, line) && i < (MAX_CASE_X * MAX_CASE_Y)) {
        if (line.length()) {
            CaseMap* cm = 0;
            char c = line.at(0);
            line = line.substr(2);
            if (c == '0') {
                cm = new CasePicross(theme);
            } else {
                cm = new CaseIcon(theme);
            }
            cm->loadFromString(line);
            map[cm->getX()][cm->getY()] = cm;
            
        }
    }
    file.close();
    
    image = Resources::getInstance()->getBackground(background);
}

void Map::refresh(Joueur* gpJoueur) {
    Audio::getInstance()->playMusic(musique);
    
    xJoueur = gpJoueur->getX();
    yJoueur = gpJoueur->getY();
    
/*
    CaseIcon* ci = dynamic_cast<CaseIcon*>(map[xJoueur][yJoueur]);
    if (ci != NULL) {
        ci->setStatus(DONE);
    }
*/    
    CaseIcon* ci = NULL;
    if (map[xJoueur][yJoueur]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[xJoueur][yJoueur]);
		ci->setStatus(DONE);
    }
    
    picrossToDo = 0;
    cumulatedTime = 0;
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            if (map[i][j] != 0) {
/* 
               CasePicross* cp = dynamic_cast<CasePicross*>(map[i][j]);
                if(cp != NULL) {
*/    
    CasePicross* cp = NULL;
    if (map[i][j]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[i][j]);

                    cp->setStatus(gpJoueur->getQuestTime(cp->getPicrossId()) ? DONE : TODO);
                    
                    if (cp->getStatus() == TODO) {
                        picrossToDo++;
                        if (cp->getRequired() != NOTHING) {
                            cp->setHasObject(gpJoueur->hasObject(cp->getRequired()));
                        }
                    } else {
                        int picrossId = cp->getPicrossId();
                        for (int k = 0; k < cp->getNbPicross(); k++) {
                            cumulatedTime += gpJoueur->getQuestTime(picrossId + k);
                        }
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            if (map[i][j] != 0) {
                toDisplay[i][j] = isToDisplay(i, j);
            }
        }
    }
}

void Map::updateCoord(Joueur* gpJoueur, int nextCase) {
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            if (map[i][j] != 0) {
/*
                CaseIcon* ci = dynamic_cast<CaseIcon*>(map[i][j]);
                if(ci != NULL) {
*/
    CaseIcon* ci = NULL;
    if (map[i][j]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[i][j]);
                    if (ci->getId() == nextCase) {
                        gpJoueur->setX(ci->getX());
                        gpJoueur->setY(ci->getY());
                        return;
                    }
                }
            }
        }
    }
}

void Map::playIconSound(int i, int j) {
    if (map[i][j] != 0) {
/*
        CaseIcon* ci = dynamic_cast<CaseIcon*>(map[i][j]);
        if(ci != NULL) {
*/
    CaseIcon* ci = NULL;
    if (map[i][j]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[i][j]);

            if (ci->getImageId() == 8) {
                Audio::getInstance()->playSound(5);
            } else if (ci->getImageId() == 9) {
                Audio::getInstance()->playSound(6);
            }
        }
    }
}

void Map::loadFromString(string line) {
    StringTokeneizer st(line);
    string value;
    
    for (int i = 0; i < 8; i++) {
        value = st.next();
        istringstream ss(value);
        int v; 
        ss >> v;
        
        switch(i) {
            case 0 : name = Text::getInstance()->getText(v); break;
            case 1 : musique = v; break;
            case 2 : background = v; break;
            case 3 : theme = v; break;
            case 4 : picrossBackground = v; break;
            case 5 : picrossTheme = v; break;
            case 6 : timeToBeat = v; break;
            case 7 : objectToWin = v; break;
        }
    }
}

bool Map::isUp(int x, int y) {
    return map[x][y]->isUp() && 
        (map[x][y]->getStatus()==DONE || map[x][y-1]->getStatus()==DONE);
}
bool Map::isDown(int x, int y) {
    return map[x][y]->isDown() && 
        (map[x][y]->getStatus()==DONE || map[x][y+1]->getStatus()==DONE) ;
}
bool Map::isLeft(int x, int y) {
    return map[x][y]->isLeft() && 
        (map[x][y]->getStatus()==DONE || map[x-1][y]->getStatus()==DONE);
}
bool Map::isRight(int x, int y) {
    return map[x][y]->isRight() 
        && (map[x][y]->getStatus()==DONE || map[x+1][y]->getStatus()==DONE);
}

int Map::getPicrossTheme() {return picrossTheme;}
int Map::getPicrossBackground() {return picrossBackground;}
int Map::getId() {return mapId;}
int Map::getPicrossToDo() {return picrossToDo;}
int Map::getCumulatedTime() {return cumulatedTime;}
int Map::getTimeToBeat() {return timeToBeat;}
int Map::getObjectToWin() {return objectToWin;}

int Map::getPicross(int x, int y) {
/*
    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getPicrossId();
    }
    return 0;
}

int Map::getNbPicross(int x, int y) {
/*
    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getNbPicross();
    }
    return 0;
}

int Map::getPicrossMusic(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getMusic();
    }
    return 0;
}

int Map::getDegats(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/

    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getDegats();
    }
    return 0;
}

int Map::getTime(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/

    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getTime();
    }
    return 0;
}

Object Map::getRequired(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getRequired();
    }
    return NOTHING;
}

Object Map::getObject(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return cp->getObject();
    }
    return NOTHING;
}

bool Map::hasObject(int x, int y) {
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        return (cp->isHasObject());
    }
    return false;
}

int Map::getDestLvl(int x, int y) {
/*    CaseIcon* ci = dynamic_cast<CaseIcon*>(map[x][y]);
    if(ci != NULL) {
*/
    CaseIcon* ci = NULL;
    if (map[x][y]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[x][y]);

        return ci->getDestLvl();
    }
    return -1;
}

int Map::getDestCase(int x, int y) {
/*    CaseIcon* ci = dynamic_cast<CaseIcon*>(map[x][y]);
    if(ci != NULL) {
*/
   CaseIcon* ci = NULL;
    if (map[x][y]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[x][y]);

        return ci->getDestCase();
    }
    return -1;
}

string Map::getText(int x, int y) {
/*    CaseIcon* ci = dynamic_cast<CaseIcon*>(map[x][y]);
    if(ci != NULL) {
*/
   CaseIcon* ci = NULL;
    if (map[x][y]->type==ID_ICON) {
        ci = static_cast<CaseIcon*>(map[x][y]);

        return Text::getInstance()->getText(ci->getDestLvl() + 1 + TEXTS_BEFORE_DEST_LVL);
    }
/*    CasePicross* cp = dynamic_cast<CasePicross*>(map[x][y]);
    if(cp != NULL) {
*/
    CasePicross* cp = NULL;
    if (map[x][y]->type==ID_PICROSS) {
        cp = static_cast<CasePicross*>(map[x][y]);

        int nbPicross = cp->getNbPicross();
        int picrossId = cp->getPicrossId();
        if (nbPicross > 1) {
            string text = Text::getInstance()->getText(TEXTS_BEFORE_COMMON + 8);
            if (cp->getStatus() == TODO) {
                return text + Text::getInstance()->getText(TEXTS_BEFORE_COMMON + 1);
            }
            int bossId = cp->getBossId();
            return text + Text::getInstance()->getText(bossId + TEXTS_BEFORE_BOSS);
        }
        
        if (cp->getStatus() == DONE) {
            string text = 
                Text::getInstance()->getText(TEXTS_BEFORE_COMMON + 11) +
                Text::getInstance()->getText(TEXTS_BEFORE_PICROSS + picrossId);
            return text;
        }
        int id = 0;
        if (!cp->isHasObject()) {
            switch (cp->getRequired()) {
                case SWORD : id = 1; break;
                case BOMBS : id = 2; break;
                case POWER_GLOVES : id = 3; break;
                case FLIPPERS : id = 4; break;
                case HOOKSHOT : id = 5; break;
                case BOW : id = 6; break;
                case HAMMER : id = 7; break;
                case KEYS : id = 8; break;
                case BOSS_KEY : id = 9; break;
                default : id = 1; break;
            }
            id += TEXTS_BEFORE_REQUIRED;
        } else {
            id = TEXTS_BEFORE_COMMON + 1;
        }
        return Text::getInstance()->getText(id);
    }
    return "";
}

bool Map::isToDisplay(int i, int j) {
    if (map[i][j]->getStatus() == DONE) {
        return true;
    }
    
    bool display = false;
    
    if (j > 0) {
        if (map[i][j-1] != 0) {
            if (map[i][j-1]->isDown() && map[i][j-1]->getStatus() == DONE) {
                map[i][j]->setForceUp(true);
                display = true;
            }
        }
    }
    if (j < MAX_CASE_Y - 1) {
        if (map[i][j+1] != 0) {
            if (map[i][j+1]->isUp() && map[i][j+1]->getStatus() == DONE) {
                map[i][j]->setForceDown(true);
                display = true;
            }
        }
    }
    if (i > 0) {
        if (map[i-1][j] != 0) {
            if (map[i-1][j]->isRight() && map[i-1][j]->getStatus() == DONE) {
                map[i][j]->setForceLeft(true);
                display = true;
            }
        }
    }
    if (i < MAX_CASE_X - 1) {
        if (map[i+1][j] != 0) {
            if (map[i+1][j]->isLeft() && map[i+1][j]->getStatus() == DONE) {
                map[i][j]->setForceRight(true);
                display = true;
            }
        }
    }
    
    if (i == xJoueur && j == yJoueur) {
        display = true;
    }
    
    return display;
}

void Map::draw(SDL_Surface* gpScreen, bool hasMap, bool hasCompas) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    for (int j = 0; j < MAX_CASE_Y; j++) {
        for (int i = 0; i < MAX_CASE_X; i++) {
            if (map[i][j] != 0) {
                bool display = toDisplay[i][j];
                if (display || hasMap) {
                    map[i][j]->draw(gpScreen, display, hasMap, hasCompas);
                }
            }
        }
    }
}
