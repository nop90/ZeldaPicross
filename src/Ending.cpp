/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Ending.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"

Ending Ending::instance=Ending();

Ending::Ending() {
    rank = 0;
}

Ending::~Ending() {
    SDL_FreeSurface(rank);
}

Ending* Ending::getInstance() {
    return &instance;
}

void Ending::init() {
    image = Resources::getInstance()->getEndingFond();
}

void Ending::start(Joueur* joueur) {
    Audio::getInstance()->playMusic(230);
    mode = TEXTS;
    Text::getInstance()->setText(TEXTS_BEFORE_ENDING + 1, 0, 0, 322, 240, 0);
    
    SDL_Surface* tree = Resources::getInstance()->getTreeFond();
    SDL_FreeSurface(rank);
    rank = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.h = 96; src.w = 48;
    for (int j=0; j<240; j+=96) {
        for (int i=0; i<320; i+=48) {
            dst.x = i; dst.y = j;
            SDL_BlitSurface(tree, &src, rank, &dst);
        }
    }
    
    string text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 2);
    Text::getInstance()->affiche(rank, text, 160 - (text.length() * 3), 10);
    
    // rank
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 3);
    text += joueur->getRank();
    Text::getInstance()->affiche(rank, text, 10, 30);
    
    // quest
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 4);
    Text::getInstance()->affiche(rank, text, 10, 50);
    
    // picross in quest mode
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 5);
    ostringstream oss;
    oss << joueur->getQuest();
    text += oss.str() + " / 200";
    Text::getInstance()->affiche(rank, text, 30, 70);
    
    // hearts
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 6);
    oss.str("");
    oss << joueur->getVieMax();
    text += oss.str() + " / 20";
    Text::getInstance()->affiche(rank, text, 30, 90);
    
    // objects
    int objects = 0;
    for (int i = 1; i < TOTAL_OBJ; i++) {
        Object obj = (Object)i;
        if (obj != KEYS && obj != HEART && joueur->hasObject(obj)) {
            objects++;
        }
    }
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 7);
    oss.str("");
    oss << objects;
    text += oss.str() + " / 14";
    Text::getInstance()->affiche(rank, text, 30, 110);
    
    // time in quest mode
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 8);
    text += Text::getInstance()->getTime(joueur->getTotalQuestTime());
    Text::getInstance()->affiche(rank, text, 30, 130);
    
    
    // free
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 9);
    Text::getInstance()->affiche(rank, text, 10, 150);
    
    // picross in free mode
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 5);
    oss.str("");
    oss << joueur->getFree();
    text += oss.str() + " / 200";
    Text::getInstance()->affiche(rank, text, 30, 170);
    
    // time in quest mode
    text = Text::getInstance()->getText(TEXTS_BEFORE_ENDING + 8);
    text += Text::getInstance()->getTime(joueur->getTotalFreeTime());
    Text::getInstance()->affiche(rank, text, 30, 190);
}


void Ending::handleEvent(Event* event) {
    if (event->QUIT) {
        event->QUIT = false;
    }
            
    if (mode == TEXTS) {
        Audio::getInstance()->stopMusic();
        mode = RANK;
    } else if (mode == RANK) {
        if (event->RETURN) {
            Audio::getInstance()->playSound(1);
            Game::getInstance()->setMode(LOGO);
        }
    }
}

void Ending::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    if (mode == TEXTS) {
        src.x = 0; src.y = 0; src.w = 320; src.h = 240;
        dst.x = 0; dst.y = 0;
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    } else if (mode == RANK) {
        src.x = 0; src.y = 0; src.w = 320; src.h = 240;
        dst.x = 0; dst.y = 0;
        SDL_BlitSurface(rank, &src, gpScreen, &dst);
    }
}
