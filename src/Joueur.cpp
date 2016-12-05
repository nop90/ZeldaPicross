/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Joueur.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"

Joueur::Joueur(int num_save) : id(num_save), loaded(false), x(3), y(3), 
map(0), vie(3), viemax(3), tutoRecords(0), tutoriel(0) {
    
    init();
    
}

Joueur::~Joueur() {
}

void Joueur::init() {
    
    loaded = false;
    x = 3; y = 3; map = 0; vie = 3; viemax = 3; tutoRecords = 0; tutoriel = 0;
    prefBg = 1; prefTh = 1; prefMs = 1;
    
    for (int i = 0; i < TOTAL_OBJ; i++) objects[i] = 0;
    for (int i = 0; i <= NB_PICROSS; i++) {
//        picross[i] = 0;
        questTime[i] = 0;
        freeTime[i] = 0;
    }
    for (int i = 0; i < NB_MAPS; i++) {
        records[i] = 0;
    }
    
    image = Resources::getInstance()->getLink();
    imageStatus = Resources::getInstance()->getStatus();
    
    load();
    
    /*
    // -> bug avec les records 6 et 9
    // -> vérifier si la comparaison pour les temps n'est pas faite avec les mauvais étages...
    for (int i = 0; i < NB_MAPS; i++) {
        cout<<i<<" : "<<records[i]<<endl;
    }
    
    objects[KEYS]=2;
    //records[4]=1;
    records[6]=1;
    records[9]=1;
    
    save();*/
    
    computeTotalQuestTime();
    computeTotalFreeTime();
}

//sauvegarde
void Joueur::save() {
    ostringstream im;
    im << id;
    ofstream f(("/3ds/ZeldaPicross/save/zpicross" + im.str() + ".dat").c_str(),ios::out | ios::binary);
    f.write((char *)&map,sizeof(int));
    f.write((char *)&x,sizeof(int));
    f.write((char *)&y,sizeof(int));
    f.write((char *)&viemax,sizeof(int));
    f.write((char *)&tutoRecords,sizeof(int));
    f.write((char *)&tutoriel,sizeof(int));
    f.write((char *)&prefBg,sizeof(int));
    f.write((char *)&prefTh,sizeof(int));
    f.write((char *)&prefMs,sizeof(int));
    for (int i = 0; i < TOTAL_OBJ; i++) f.write((char *)&objects[i],sizeof(int));
    for (int i = 0; i <= NB_PICROSS; i++) f.write((char *)&questTime[i],sizeof(int));
    for (int i = 0; i <= NB_PICROSS; i++) f.write((char *)&freeTime[i],sizeof(int));
    for (int i = 0; i < NB_MAPS; i++) f.write((char *)&records[i],sizeof(int));
    f.close();
    
    Game* game = Game::getInstance();
    game->setRank(getRankNumber());
    game->setQuest(quest);
    game->setFree(free);
    if (quest == 200) game->setQuestTime(totalQuestTime);
    if (free == 200) game->setFreeTime(totalFreeTime);
    game->saveSystem();
    
    loaded = true;
}

void Joueur::load() {
    ostringstream im;
    im << id;
    ifstream f(("/3ds/ZeldaPicross/save/zpicross" + im.str() + ".dat").c_str(),ios::in | ios::binary);
    if(!f.is_open()) return;
    f.read((char *)&map,sizeof(int));
    f.read((char *)&x,sizeof(int));
    f.read((char *)&y,sizeof(int));
    f.read((char *)&viemax,sizeof(int)); vie = viemax;
    f.read((char *)&tutoRecords,sizeof(int));
    f.read((char *)&tutoriel,sizeof(int));
    f.read((char *)&prefBg,sizeof(int));
    f.read((char *)&prefTh,sizeof(int));
    f.read((char *)&prefMs,sizeof(int));
    for (int i = 0; i < TOTAL_OBJ; i++) f.read((char *)&objects[i],sizeof(int));
    for (int i = 0; i <= NB_PICROSS; i++) f.read((char *)&questTime[i],sizeof(int));
    for (int i = 0; i <= NB_PICROSS; i++) f.read((char *)&freeTime[i],sizeof(int));
    for (int i = 0; i < NB_MAPS; i++) f.read((char *)&records[i],sizeof(int));
    f.close();
    loaded = true;
    
    
}

void Joueur::moveX(int m) {x+=m;}
void Joueur::moveY(int m) {y+=m;}

int Joueur::getX() {return x;}
int Joueur::getY() {return y;}
int Joueur::getMap() {return map;}
int Joueur::getVie() {return vie;}
int Joueur::getVieMax() {return viemax;}
bool Joueur::hasObject(Object obj) {return objects[obj];}
int Joueur::getQuestTime(int i) {return questTime[i];}
int Joueur::getFreeTime(int i) {return freeTime[i];}
int Joueur::getTotalQuestTime() {return totalQuestTime;}
int Joueur::getTotalFreeTime() {return totalFreeTime;}
int Joueur::getTutoriel() {return tutoriel;}
bool Joueur::isLoaded() {return loaded;}

int Joueur::getPrefBg() {return prefBg;}
int Joueur::getPrefTh() {return prefTh;}
int Joueur::getPrefMs() {return prefMs;}
void Joueur::setPrefBg(int i) {prefBg = i;}
void Joueur::setPrefTh(int i) {prefTh = i;}
void Joueur::setPrefMs(int i) {prefMs = i;}

void Joueur::setTutoriel(int i) {tutoriel = i;}
void Joueur::setX(int i) {x = i;}
void Joueur::setY(int i) {y = i;}
void Joueur::setMap(int i) {map = i;}
void Joueur::setObject(Object obj) {objects[(int)obj]++;}
void Joueur::setVie(int i) {
    int degat = vie - i;
    vie = i;
    if (degat > 0 && vie <= degat && vie > 0) {
        Audio::getInstance()->playSound(14);
    }
    if (vie < 0) vie = 0;
    else if (vie > viemax) vie = viemax;
}
void Joueur::setVieMax(int i) {
    viemax = i;
    if (viemax < 0) viemax = 0;
    else if (viemax > 20) viemax = 20;
}
void Joueur::setQuestTime(int i, int t) {
    if (questTime[i] == 0 || t < questTime[i]) {
        questTime[i] = t;
        computeTotalQuestTime();
    }
}
void Joueur::setFreeTime(int i, int t) {
    if (freeTime[i] == 0 || t < freeTime[i]) {
        freeTime[i] = t;
        computeTotalFreeTime();
    }
}

int Joueur::getRecord(int i) {
    if (i >= 0 && i < NB_MAPS) {
        return records[i - 1];
    }
    return -1;
}

void Joueur::setRecord(int i) {
    if (i >= 0 && i < NB_MAPS) {
        records[i - 1] = 1;
    }
}

int Joueur::getTutoRecords() {return tutoRecords;}

void Joueur::setTutoRecords() {tutoRecords = 1;}

void Joueur::removeVie(int degats) {
    int deg = degats;
    if (objects[SHIELD]) {
        deg = degats / 2;
        if (deg < 1) {
            deg = 1;
        }
    }
    setVie(vie - deg);
}

void Joueur::useKey() {
    objects[KEYS] = objects[KEYS] - 1;
    if (objects[KEYS] < 0) objects[KEYS] = 0;
}

void Joueur::computeTotalQuestTime() {
    totalQuestTime = 0;
    for (int i = 0; i <= NB_PICROSS; i++) totalQuestTime += questTime[i];
}

void Joueur::computeTotalFreeTime() {
    totalFreeTime = 0;
    for (int i = 0; i <= NB_PICROSS; i++) totalFreeTime += freeTime[i];
}

int Joueur::getRankNumber() {
    int score = 0;
    int scoreMax = 433;
    
    // all picross are done
    bool allQuest = true;
    bool allFree = true;
    
    // do each picross
    quest = 0;
    free = 0;
    for (int i = 1; i <= 200; i++) {
        if (getQuestTime(i)) {
            quest++;
        } else {
            allQuest = false;
        }
        if (getFreeTime(i)) {
            free++;
        } else {
            allFree = false;
        }
    }
    score += quest;
    score += free;
    
    //get each objet
    for (int i = 1; i < TOTAL_OBJ; i++) {
        Object obj = (Object)i;
        if (obj != KEYS && obj != HEART && hasObject(obj)) {
            score++;
        }
    }
    
    //get each heart
    score += viemax - 3;
    
    //have good times
    if (allQuest && getTotalQuestTime() < 86400) {
        score++;
    }
    if (allFree && getTotalFreeTime() < 86400) {
        score++;
    }
    
    return (score * (NB_RANK - 1)) / scoreMax;
}

string Joueur::getRank() {
    char c = 64 + NB_RANK - getRankNumber();
    ostringstream oss;
    oss << c;
    return oss.str();
}

int Joueur::getQuest() {
    return quest;
}

int Joueur::getFree() {
    return free;
}

void Joueur::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = 14; 
    src.h = 12;
    src.x = 0;
    src.y = 0;
    dst.x = COORD_MAP_X + TOTAL_CASE_MAP_SIZE * x + PATH_CASE_MAP_SIZE + 1;
    dst.y = COORD_MAP_Y + TOTAL_CASE_MAP_SIZE * y + PATH_CASE_MAP_SIZE + 2; 
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawLife(SDL_Surface* gpScreen, int i, int j) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = 8; 
    src.h = 8;
    src.y = 24;
    dst.x = i;
    dst.y = j;
    for (int k = 0; k < viemax; k++) {
        if (k == 10) {dst.x = i; dst.y += src.h;}
        src.x = (vie > k) ? 16 : 24;
        SDL_BlitSurface(imageStatus, &src, gpScreen, &dst);
        dst.x += src.w;
    }
}

void Joueur::drawObjects(SDL_Surface* gpScreen, int i, int j) {
    int a = i;
    int b = j;
    int w = 8;
    int h = 8;
    for (int k = 0; k < 10; k++) {
        if (!objects[k+1]) {
            continue;
        }
        drawObject(gpScreen, a, b, k+1);
        a += w;
    }
    a = i;
    b = j + h + 1;
    if (objects[MAP]) {
        drawObject(gpScreen, a, b, MAP);
        a += w;
    }
    if (objects[COMPASS]) {
        drawObject(gpScreen, a, b, COMPASS);
        a += w;
    }
    if (objects[BOSS_KEY]) {
        drawObject(gpScreen, a, b, BOSS_KEY);
        a += w;
    }
    if (objects[TRIFORCE]) {
        drawObject(gpScreen, a, b, TRIFORCE);
        a += w;
    }
    for (int k = 0; k < objects[KEYS]; k++) {
        drawObject(gpScreen, a, b, KEYS);
        a += w;
    }
}

void Joueur::drawObject(SDL_Surface* gpScreen, int i, int j, int obj) {
    SDL_Rect src;
    SDL_Rect dst;
    src.w = 8; 
    src.h = 8;
    dst.x = i;
    dst.y = j;
    
    switch (obj) {
        case SWORD : src.x=0; src.y=0; break;
        case BOMBS : src.x=8; src.y=0; break;
        case POWER_GLOVES : src.x=16; src.y=0; break;
        case FLIPPERS : src.x=0; src.y=8; break;
        case HOOKSHOT : src.x=8; src.y=8; break;
        case BOW : src.x=16; src.y=8; break;
        case HAMMER : src.x=24; src.y=8; break;
        case BOOTS : src.x=0; src.y=16; break;
        case SHIELD : src.x=8; src.y=16; break;
        case BOOK : src.x=16; src.y=16; break;
        case MAP : src.x=0; src.y=24; break;
        case COMPASS : src.x=8; src.y=24; break;
        case BOSS_KEY : src.x=24; src.y=16; break;
        case KEYS : src.x=24; src.y=0; break;
        case HEART : src.x=16; src.y=24; break;
        case TRIFORCE : src.x=32; src.y=24; break;
        default : return;
    }
    SDL_BlitSurface(imageStatus, &src, gpScreen, &dst);
}
