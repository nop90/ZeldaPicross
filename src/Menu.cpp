/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Menu.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"

Menu Menu::instance=Menu();

Menu::Menu() {
    image = 0;
    imageMode = 0;
    imageProfil = 0;
    imageBaseProfil = 0;
    imageDelete = 0;
    line = 0;
    column = 0;
    lineFee = 0;
    lastProfil = -1;
}

Menu::~Menu() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(imageMode);
    SDL_FreeSurface(imageProfil);
    SDL_FreeSurface(imageBaseProfil);
    SDL_FreeSurface(imageDelete);
}

void Menu::init(Joueur* j[]) {
    curseurLink = Resources::getInstance()->getLinkMenu();
    curseurFee = Resources::getInstance()->getFeeMenu();
    
    for (int i = 0; i < 3; i++) {
        joueurs[i] = j[i];
    }
}

void Menu::cadre(int x, int y, int w, int h, SDL_Surface* im) {
    SDL_Surface* imageTmp = Resources::getInstance()->getCadreMenu();
    
    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    
    for (int j = y+16; j < y+h-16; j+=16)
        for (int i = x+16; i < x+w-16; i+=16) {
            src.x = 16; src.y = 64; dst.x = i; dst.y = j; 
            SDL_BlitSurface(imageTmp, &src, im, &dst);
        }
    
    //haut et bas
    for (int i = x+16; i < x+w-16; i+=16) {
        src.x = 16; src.y = 48; dst.x = i; dst.y = y; 
        SDL_BlitSurface(imageTmp, &src, im, &dst);
        src.x = 16; src.y = 80; dst.x = i; dst.y = y+h-16; 
        SDL_BlitSurface(imageTmp, &src, im, &dst);
    }
    
    //gauche et droite
    for (int j = y+16; j < y+h-16; j+=16) {
        src.x = 0; src.y = 64; dst.x = x; dst.y = j; 
        SDL_BlitSurface(imageTmp, &src, im, &dst);
        src.x = 32; src.y = 64; dst.x = x+w-16; dst.y = j;
        SDL_BlitSurface(imageTmp, &src, im, &dst);
    }
    
    //haut gauche
    src.x = 0; src.y = 48; dst.x = x; dst.y = y; 
    SDL_BlitSurface(imageTmp, &src, im, &dst);
    
    //haut droite
    src.x = 32; src.y = 48; dst.x = x+w-16; dst.y = y; 
    SDL_BlitSurface(imageTmp, &src, im, &dst);
    
    //bas gauche
    src.x = 0; src.y = 80; dst.x = x; dst.y = y+h-16; 
    SDL_BlitSurface(imageTmp, &src, im, &dst);
    
    //bas droite
    src.x = 32; src.y = 80; dst.x = x+w-16; dst.y = y+h-16; 
    SDL_BlitSurface(imageTmp, &src, im, &dst);
}

void Menu::start() {
    mode = SELECT_SAVE;
    Audio::getInstance()->playMusic(190);
    
    
    //mode
    if (imageMode == NULL) {
        imageMode = SDL_CreateRGBSurface(SDL_HWSURFACE, 112+16, 72+32+16, 32, 0, 0, 0, 0);
        cadre(0, 0, 112+16, 72+32+16, imageMode);
        string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 19);
        Text::getInstance()->affiche(imageMode, text, 29, 12);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 20);
        Text::getInstance()->affiche(imageMode, text, 29, 32);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 21);
        Text::getInstance()->affiche(imageMode, text, 29, 52);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 15);
        Text::getInstance()->affiche(imageMode, text, 29, 72);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 14);
        Text::getInstance()->affiche(imageMode, text, 29, 92);
    }
    
    //profil
    if (imageBaseProfil == NULL) {
        imageBaseProfil = SDL_CreateRGBSurface(SDL_HWSURFACE, 112+16*6, 72+32+16, 32, 0, 0, 0, 0);
        cadre(0, 0, 112+16*6, 72+32+16, imageBaseProfil);
    }
    
    //erase
    if (imageDelete == NULL) {
        imageDelete = SDL_CreateRGBSurface(SDL_HWSURFACE, 112, 72, 32, 0, 0, 0, 0);
        cadre(0, 0, 112, 72, imageDelete);
        string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 16);
        Text::getInstance()->affiche(imageDelete, text, 29, 9);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 17);
        Text::getInstance()->affiche(imageDelete, text, 47, 29);
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 18);
        Text::getInstance()->affiche(imageDelete, text, 47, 49);
    }
    
    
    
    // build background
    SDL_Surface* imageTmp = Resources::getInstance()->getCadreMenu();
    if (image != NULL) SDL_FreeSurface(image);
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 16; src.y = 16; src.w = 16; src.h = 16;
    for (int j = 0; j < 240; j+=16) {
        for (int i = 0; i < 320; i+=16) {
            dst.x = i; dst.y = j;
            SDL_BlitSurface(imageTmp, &src, image, &dst);
        }
    }
    
    // title
    string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 1);
    int width = text.length() * 6;
    
    src.x = 0; src.y = 0; dst.x = 16; dst.y = 0; 
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    src.x = 0; src.y = 16; dst.x = 16; dst.y = 16; 
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    src.x = 0; src.y = 32; dst.x = 16; dst.y = 32;
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    
    for (int i = 0; i <= width / 16; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageTmp, &src, image, &dst);
    }
    
    int newX = 48+16*((int)(width/16));
    src.x = 32; src.y = 0; dst.x = newX; dst.y = 0; 
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = newX; dst.y = 16; 
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = newX; dst.y = 32; 
    SDL_BlitSurface(imageTmp, &src, image, &dst);
    
    Text::getInstance()->affiche(image, text, 32, 16);
    
    
    // saves
    cadre(16,48,288,32, image);
    cadre(16,96,288,32, image);
    cadre(16,144,288,32, image);
    
    Text::getInstance()->affiche(image, "1.", 44, 56);
    Text::getInstance()->affiche(image, "2.", 44, 104);
    Text::getInstance()->affiche(image, "3.", 44, 152);
    
    for (int i = 0; i < 3; i++) {
        Joueur* gpJoueur = joueurs[i];
        loaded[i] = gpJoueur->isLoaded();
        if (gpJoueur->isLoaded()) {
            gpJoueur->drawLife(image, 48+16, 57 + i*48);
            gpJoueur->drawObjects(image, 48+8+16*6, 57 + i*48);
            text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 2);
            text += gpJoueur->getRank();
            Text::getInstance()->affiche(image, text, 48+16*12, 57 + i*48);
        }
    }
        
    // options and records
    cadre(16,192,136,32, image);
    cadre(168,192,136,32, image);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 3);
    Text::getInstance()->affiche(image, text, 63, 200);
    text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 4);
    Text::getInstance()->affiche(image, text, 215, 200);
}

Menu* Menu::getInstance() {
    return &instance;
}

void Menu::initProfil() {
    if (line != lastProfil) {
        SDL_FreeSurface(imageProfil);
        imageProfil = SDL_CreateRGBSurface(SDL_HWSURFACE, 112+16*6, 72+32+16, 32, 0, 0, 0, 0);
        
        SDL_Rect src;
        SDL_Rect dst;
        src.x = 0; src.y = 0; src.w = 112+16*6; src.h = 72+32+16; dst.x = 0; dst.y = 0;
        SDL_BlitSurface(imageBaseProfil, &src, imageProfil, &dst);
        
        ostringstream oss;
        string text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 2);
        text += joueurs[line]->getRank();
        Text::getInstance()->affiche(imageProfil, text, 12, 12);
        
        oss << joueurs[line]->getQuest();
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 22) + oss.str();
        Text::getInstance()->affiche(imageProfil, text, 12, 32);
        oss.str("");
        
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 23);
        text += Text::getInstance()->getTime(joueurs[line]->getTotalQuestTime());
        Text::getInstance()->affiche(imageProfil, text, 12, 52);
        
        oss << joueurs[line]->getFree();
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 24) + oss.str();
        Text::getInstance()->affiche(imageProfil, text, 12, 72);
        
        text = Text::getInstance()->getText(TEXTS_BEFORE_MENU + 25);
        text += Text::getInstance()->getTime(joueurs[line]->getTotalFreeTime());
        Text::getInstance()->affiche(imageProfil, text, 12, 92);
        
        lastProfil = line;
    }
}

void Menu::handleEvent(Event* event) {
    ostringstream oss;
    
    if (event->RETURN) {
        if (line < 3) {
            switch (mode) {
                case SELECT_SAVE :
                    Audio::getInstance()->playSound(1);
                    if (loaded[line]) {
                        mode = SELECT_MODE;
                        lineFee = 0;
                    } else {
                        Audio::getInstance()->stopMusic();
                        Game::getInstance()->setSave(line + 1);
                        Game::getInstance()->setMode(INTRO);
                    }
                    break;
                case SELECT_MODE :
                    switch (lineFee) {
                        case 0 : 
                            Audio::getInstance()->stopMusic();
                            Audio::getInstance()->playSound(1);
                            Game::getInstance()->setSave(line + 1);
                            Game::getInstance()->setMode(QUEST);
                            lastProfil = -1;
                            break;
                        case 1 :
                            Audio::getInstance()->stopMusic();
                            Audio::getInstance()->playSound(1);
                            Game::getInstance()->setSave(line + 1);
                            Game::getInstance()->setMode(FREE);
                            lastProfil = -1;
                            break;
                        case 2 :
                            Audio::getInstance()->playSound(1);
                            initProfil();
                            mode = PROFIL;
                            break;
                        case 3 :
                            Audio::getInstance()->playSound(1);
                            mode = DELETE;
                            lineFee = 1;
                            break;
                        case 4 :
                            mode = SELECT_SAVE;
                            Audio::getInstance()->playSound(2);
                            break;
                    }
                    break;
                case PROFIL :
                    mode = SELECT_MODE;
                    Audio::getInstance()->playSound(2);
                    break;
                case DELETE :
                    switch (lineFee) {
                        case 0 : // yes
                            Audio::getInstance()->playSound(2);
                            oss << (line+1);
                            remove(("/3ds/ZeldaPicross/save/zpicross" + oss.str() 
                                + ".dat").c_str());
                            joueurs[line]->init();
                            start();
                            break;
                        case 1 : // no
                            Audio::getInstance()->playSound(2);
                            mode = SELECT_MODE;
                            lineFee = 2;
                            break;
                    }
                    break;
            }
        } else if (line == 3) {
            if (column == 0) {
                Audio::getInstance()->playSound(1);
                Game::getInstance()->setMode(OPTIONS);
            } else if (column == 1) {
                Audio::getInstance()->playSound(1);
                Game::getInstance()->setMode(RECORDS);
            }
        }
        return;
    }
    
    switch (mode) {
        case SELECT_SAVE :
            if (event->UP) {
                line--;
                if (line < 0) {
                    line = 3;
                }
                column = 0;
                Audio::getInstance()->playSound(3);
            }
            if (event->DOWN) {
                line++;
                if (line > 3) {
                    line = 0;
                }
                column = 0;
                Audio::getInstance()->playSound(3);
            }
            if (event->LEFT) {
                if (line == 3) {
                    column--;
                    if (column < 0) {
                        column = 1;
                    }
                    Audio::getInstance()->playSound(3);
                }
            }
            if (event->RIGHT) {
                if (line == 3) {
                    column++;
                    if (column > 1) {
                        column = 0;
                    }
                    Audio::getInstance()->playSound(3);
                }
            }
            break;
        case SELECT_MODE :
            if (event->UP) {
                lineFee--;
                if (lineFee < 0) {
                    lineFee = 4;
                }
                Audio::getInstance()->playSound(3);
            }
            if (event->DOWN) {
                lineFee++;
                if (lineFee > 4) {
                    lineFee = 0;
                }
                Audio::getInstance()->playSound(3);
            }
            break;
        case DELETE :
            if (event->UP) {
                lineFee--;
                if (lineFee < 0) {
                    lineFee = 1;
                }
                Audio::getInstance()->playSound(3);
            }
            if (event->DOWN) {
                lineFee++;
                if (lineFee > 1) {
                    lineFee = 0;
                }
                Audio::getInstance()->playSound(3);
            }
            break;
    }
}

void Menu::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.w = 320; src.h = 240;
    dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    //curseurLink
    src.x = 0; src.y = 0; src.w = 16; src.h = 21;
    dst.x = 26 + 152 * column; dst.y = 53 + 48 * line;
    SDL_BlitSurface(curseurLink, &src, gpScreen, &dst);
    
    if (mode == SELECT_MODE || mode == DELETE) {
        src.x = 0; src.y = 0; src.w = 112+16; src.h = 72+32+16;
        dst.x = 96; dst.y = 68-8;
        SDL_BlitSurface(imageMode, &src, gpScreen, &dst);
        
        if (mode == SELECT_MODE) {
            src.h = 16; src.w = 16; src.x = 0; src.y=0;
            dst.x = 104; dst.y = 80 - 8 + 20 * lineFee;
            SDL_BlitSurface(curseurFee, &src, gpScreen, &dst);
        } else if (mode == DELETE) {
            src.x = 0; src.y = 0; src.w = 112; src.h = 72;
            dst.x = 104; dst.y = 84;
            SDL_BlitSurface(imageDelete, &src, gpScreen, &dst);
            
            src.h = 16; src.w = 16; src.x = 0; src.y=0;
            dst.x = 112; dst.y = 113 + 20 * lineFee;
            SDL_BlitSurface(curseurFee, &src, gpScreen, &dst);
        }
    } else if (mode == PROFIL) {
        src.x = 0; src.y = 0; src.w = 112+16*6; src.h = 72+32+16;
        dst.x = 56; dst.y = 68-8;
        SDL_BlitSurface(imageProfil, &src, gpScreen, &dst);
    }
    
}
