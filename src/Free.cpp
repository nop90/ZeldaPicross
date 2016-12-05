/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Free.h"
#include "Resources.h"
#include "Game.h"
#include "Audio.h"
#include "Text.h"
#include "Chrono.h"

Free Free::instance=Free();

Free::Free() {
    image = 0;
    imagePage = 0;
    imageOptions = 0;
    background = 0;
    gpPicross = Picross::getInstance();
}

Free::~Free() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(imagePage);
    SDL_FreeSurface(imageOptions);
}

Free* Free::getInstance() {
    return &instance;
}

void Free::init() {
    curseurLink = Resources::getInstance()->getLinkMenu();
    
    SDL_Surface* imageTmp = Resources::getInstance()->getCadreMenu();
    
    if (image == NULL) {
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
        
        
    
    
    
        // cadres
        cadre(16,44,288,32, image);
        cadre(16,84,288,32, image);
        cadre(16,124,288,32, image);
        cadre(16,164,288,32, image);
        cadre(16,204,288,32, image);
        
        src.x = 0; src.y = 0; src.w = 320; src.h = 240;
        dst.x = 0; dst.y = 0;
        imageOptions = SDL_CreateRGBSurface(
            SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
        SDL_BlitSurface(image, &src, imageOptions, &dst);
        src.w = 16; src.h = 16;
        
        // title for image
        string text = Text::getInstance()->getText(TEXTS_BEFORE_FREE + 1);
        text += " - " + Text::getInstance()->getText(TEXTS_BEFORE_FREE + 2);
        text += " ";
        int width = (text.length() + 7) * 6;
    
        src.x = 0; src.y = 0; dst.x = 16; dst.y = 0; src.h = 16;
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 0; src.y = 16; dst.x = 16; dst.y = 16; 
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 0; src.y = 32; dst.x = 16; dst.y = 32; src.h = 16 - 4;
        SDL_BlitSurface(imageTmp, &src, image, &dst);
    
        for (int i = 0; i <= width / 16; i++) {
            src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; src.h = 16;
            SDL_BlitSurface(imageTmp, &src, image, &dst);
            src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
            SDL_BlitSurface(imageTmp, &src, image, &dst);
            src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; src.h = 16 - 4;
            SDL_BlitSurface(imageTmp, &src, image, &dst);
        }
    
        int newX = 48+16*((int)(width/16));
        src.x = 32; src.y = 0; dst.x = newX; dst.y = 0; src.h = 16;
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 32; src.y = 16; dst.x = newX; dst.y = 16; 
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        src.x = 32; src.y = 32; dst.x = newX; dst.y = 32; src.h = 16 - 4;
        SDL_BlitSurface(imageTmp, &src, image, &dst);
        
        cadre(16+16*14,8,288-16*14,32, image);
        
        
        
        // title for imageOptions
        text = Text::getInstance()->getText(TEXTS_BEFORE_FREE + 1);
        width = text.length() * 6;
    
        src.x = 0; src.y = 0; dst.x = 16; dst.y = 0; src.h = 16;
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        src.x = 0; src.y = 16; dst.x = 16; dst.y = 16; 
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        src.x = 0; src.y = 32; dst.x = 16; dst.y = 32; src.h = 16 - 4;
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
    
        for (int i = 0; i <= width / 16; i++) {
            src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; src.h = 16;
            SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
            src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
            SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
            src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; src.h = 16 - 4;
            SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        }
        
        newX = 48+16*((int)(width/16));
        src.x = 32; src.y = 0; dst.x = newX; dst.y = 0; src.h = 16;
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        src.x = 32; src.y = 16; dst.x = newX; dst.y = 16; 
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        src.x = 32; src.y = 32; dst.x = newX; dst.y = 32; src.h = 16 - 4;
        SDL_BlitSurface(imageTmp, &src, imageOptions, &dst);
        
        int realWidth = (((int)(width / 16)) + 1) * 16;
        int offset = (realWidth - width) / 2;
        if (offset == 16) {
            offset = 8;
        }
        
        Text::getInstance()->affiche(imageOptions, text, 30 + offset, 16);
        
        for (int i = 0; i < 5; i++) {
            text = Text::getInstance()->getText(TEXTS_BEFORE_FREE + 3 + i);
            width = text.length() * 6;
            Text::getInstance()->affiche(imageOptions, 
                text, 160 - (width / 2), 52 + 40 * i);
        }
    }
    
}

void Free::cadre(int x, int y, int w, int h, SDL_Surface* im) {
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

void Free::start(Joueur* joueur) {
    gpJoueur = joueur;
    
    if (gpJoueur->getPrefBg() <= NB_BACKGROUND) {
        background = Resources::getInstance()->getPicrossBG(
            gpJoueur->getPrefBg());
    }
    
    //gpJoueur->init();
    gpPicross->useKeyboard();
    Audio::getInstance()->playMusic(195);
    mode = FREE_MAIN;
    page = 0;
    line = 0;
    lineOpt = 0;
    picrossId = 0;
    picrossVictory = false;
    quitPicross = false;
    quit = false;
    
    initPage();
}

void Free::initPage() {
    if (imagePage) SDL_FreeSurface(imagePage);
    
    imagePage = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    
    SDL_Rect src;
    SDL_Rect dst;
        
    src.x = 0; src.y = 0; src.w = 320; src.h = 240;
    dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, &src, imagePage, &dst);
    
    ostringstream oss;
    string text = Text::getInstance()->getText(TEXTS_BEFORE_FREE + 1);
    text += " - " + Text::getInstance()->getText(TEXTS_BEFORE_FREE + 2);
    text += " ";
    if (page + 1 < 10) {
        text += "0";
    }
    oss << (page + 1) << " / " << NB_PAGES;
    text += oss.str();
    
    int width = text.length() * 6;
    int realWidth = (((int)(width / 16)) + 1) * 16;
    int offset = (realWidth - width) / 2;
    if (offset == 16) {
        offset = 8;
    }
    
    Text::getInstance()->affiche(imagePage, text, 30 + offset, 16);
    
    text = Text::getInstance()->getTime(gpJoueur->getTotalFreeTime());
    Text::getInstance()->affiche(imagePage, text, 44 + 203, 16);
    
    for (int i = 0; i < MAX_BY_PAGE; i++) {
        oss.str("");
        int number = 1 + i + page * MAX_BY_PAGE;
        if (number < 10) {
            oss << "0";
        }
        if (number < 100) {
            oss << "0";
        }
        oss << number;
        text = oss.str() + ".";
        
        if (gpJoueur->getFreeTime(number)) {
            text += Text::getInstance()->getText(
                TEXTS_BEFORE_PICROSS + number);
        } else {
            text += Text::getInstance()->getText(TEXTS_BEFORE_COMMON + 1);
        }
        
        Text::getInstance()->affiche(imagePage, text, 44, 52 + 40 * i);
        
        text = Text::getInstance()->getTime(gpJoueur->getFreeTime(number));
        Text::getInstance()->affiche(imagePage, text, 44 + 203, 52 + 40 * i);
    }
    
}

void Free::handleEvent(Event* event) {
    
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
            mode = SELECT_PICROSS;
            Chrono::getInstance()->stop();
            Chrono::getInstance()->raz();
            Audio::getInstance()->playMusic(195);
            if (lineOpt == 1) {
                initPage();
            }
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
            case FREE_MAIN :
                Text::getInstance()->setText(TEXTS_BEFORE_COMMON + 4, 
                    8, 152, 304, 80, 3);
                quit = true;
                break;
            case SELECT_PICROSS :
                mode = FREE_MAIN;
                Audio::getInstance()->playSound(2);
                event->ESCAPE = false;
                break;
            case FREE_PICROSS :
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
    
    
    if (mode == FREE_MAIN) {
        int backgroundId = 0;
        
        if (event->RETURN) {
            if (lineOpt == 0) {
                mode = SELECT_PICROSS;
                Audio::getInstance()->playSound(1);
                return;
            } else if (lineOpt == 1) {
                picrossId = randomId();
                page = (picrossId - 1) / MAX_BY_PAGE;
                line = (picrossId - 1) % MAX_BY_PAGE;
                launchPicross();
                return;
            }
        }
    
        if (event->UP) {
            lineOpt--;
            if (lineOpt < 0) {
                lineOpt = 4;
                if (gpJoueur->getPrefMs() <= NB_MUSIC) {
                    Audio::getInstance()->playMusic(gpJoueur->getPrefMs());
                }
            }
            if (lineOpt == 3) {
                Audio::getInstance()->playMusic(195);
            }
            Audio::getInstance()->playSound(3);
        }
        if (event->DOWN) {
            lineOpt++;
            if (lineOpt > 4) {
                lineOpt = 0;
                Audio::getInstance()->playMusic(195);
            }
            if (lineOpt == 4 && gpJoueur->getPrefMs() <= NB_MUSIC) {
                Audio::getInstance()->playMusic(gpJoueur->getPrefMs());
            }
            Audio::getInstance()->playSound(3);
        }
        if (event->LEFT) {
            switch (lineOpt) {
                case 2 :
                    gpJoueur->setPrefBg(gpJoueur->getPrefBg() - 1);
                    if (gpJoueur->getPrefBg() < 1) {
                        gpJoueur->setPrefBg(NB_BACKGROUND + 1);
                    }
                    if (gpJoueur->getPrefBg() <= NB_BACKGROUND) {
                        background = Resources::getInstance()->getPicrossBG(
                            gpJoueur->getPrefBg());
                    }
                    Audio::getInstance()->playSound(3);
                    break;
                case 3 :
                    gpJoueur->setPrefTh(gpJoueur->getPrefTh() - 1);
                    if (gpJoueur->getPrefTh() < 1) {
                        gpJoueur->setPrefTh(NB_THEME + 1);
                    }
                    Audio::getInstance()->playSound(3);
                    break;
                case 4 :
                    gpJoueur->setPrefMs(gpJoueur->getPrefMs() - 1);
                    if (gpJoueur->getPrefMs() < 1) {
                        gpJoueur->setPrefMs(NB_MUSIC + 1);
                    }
                    Audio::getInstance()->playSound(3);
                    if (gpJoueur->getPrefMs() <= NB_MUSIC) {
                        Audio::getInstance()->playMusic(gpJoueur->getPrefMs());
                    } else {
                        Audio::getInstance()->playMusic(195);
                    }
                    break;
                default :
                    break;
            }
        }
        if (event->RIGHT) {
            switch (lineOpt) {
                case 2 :
                    gpJoueur->setPrefBg(gpJoueur->getPrefBg() + 1);
                    if (gpJoueur->getPrefBg() > NB_BACKGROUND + 1) {
                        gpJoueur->setPrefBg(1);
                    }
                    if (gpJoueur->getPrefBg() <= NB_BACKGROUND) {
                        background = Resources::getInstance()->getPicrossBG(
                            gpJoueur->getPrefBg());
                    }
                    Audio::getInstance()->playSound(3);
                    break;
                case 3 :
                    gpJoueur->setPrefTh(gpJoueur->getPrefTh() + 1);
                    if (gpJoueur->getPrefTh() > NB_THEME + 1) {
                        gpJoueur->setPrefTh(1);
                    }
                    Audio::getInstance()->playSound(3);
                    break;
                case 4 :
                    gpJoueur->setPrefMs(gpJoueur->getPrefMs() + 1);
                    if (gpJoueur->getPrefMs() > NB_MUSIC + 1) {
                        gpJoueur->setPrefMs(1);
                    }
                    Audio::getInstance()->playSound(3);
                    if (gpJoueur->getPrefMs() <= NB_MUSIC) {
                        Audio::getInstance()->playMusic(gpJoueur->getPrefMs());
                    } else {
                        Audio::getInstance()->playMusic(195);
                    }
                    break;
                default :
                    break;
            }
        }
        
    } else if (mode == SELECT_PICROSS) {
        
        if (event->RETURN) {
            picrossId = 1 + line + page * MAX_BY_PAGE;
            launchPicross();
            return;
        }
    
        if (event->UP) {
            line--;
            if (line < 0) {
                line = MAX_BY_PAGE - 1;
            }
            Audio::getInstance()->playSound(3);
        }
        if (event->DOWN) {
            line++;
            if (line >= MAX_BY_PAGE) {
                line = 0;
            }
            Audio::getInstance()->playSound(3);
        }
        if (event->LEFT) {
            page--;
            if (page < 0) {
                page = NB_PAGES - 1;
            }
            Audio::getInstance()->playSound(3);
            initPage();
        }
        if (event->RIGHT) {
            page++;
            if (page >= NB_PAGES) {
                page = 0;
            }
            Audio::getInstance()->playSound(3);
            initPage();
        }
    
    } else if (mode == FREE_PICROSS) {
        
        gpPicross->handleEvent(event);
        
    }
}

void Free::launchPicross() {
    picrossVictory = false;
    mode = FREE_PICROSS;
    
    if (gpJoueur->getPrefBg() > NB_BACKGROUND) {
        background = Resources::getInstance()->getPicrossBG(
            1 + (rand() % NB_BACKGROUND));
    }
    
    int themeId = gpJoueur->getPrefTh() <= NB_THEME ? 
        gpJoueur->getPrefTh() : 1 + (rand() % NB_THEME);
    SDL_Surface* theme = 
        Resources::getInstance()->getTheme(themeId);
    
    gpPicross->init(Resources::getInstance()->getPicross(picrossId), 
        theme, LIBRE);
    
    int musicId = gpJoueur->getPrefMs() <= NB_MUSIC ? 
        gpJoueur->getPrefMs() : 1 + (rand() % NB_MUSIC);
    Audio::getInstance()->playMusic(musicId);
    Chrono::getInstance()->start();
}

int Free::randomId() {
    
    int total = gpJoueur->getFree();
    
    if (total == NB_PICROSS) {
        return 1 + (rand() % NB_PICROSS);
    }
    
    int ids[200];
    int current = 0;
    
    for (int i = 1; i <= 200; i++) {
        if (!gpJoueur->getFreeTime(i)) {
            ids[current++] = i;
        }
    }
    return ids[rand() % current];
}

void Free::loop() {
    
    if (mode == FREE_PICROSS) {
        if (gpPicross->isVictory() && gpJoueur->getVie()) {
            if (picrossVictory) {
                if (!Text::getInstance()->isToDisplay()) {
                    mode = SELECT_PICROSS;
                    Chrono::getInstance()->raz();
                    Audio::getInstance()->playMusic(195);
                    initPage();
                }
            } else {
                Text::getInstance()->setText(
                    TEXTS_BEFORE_PICROSS + picrossId, 104, 8, 208, 32, 2);
                Audio::getInstance()->playSound(4);
                picrossVictory = true;
                Chrono::getInstance()->stop();
                gpJoueur->setFreeTime(picrossId, 
                        Chrono::getInstance()->getTime());
            }
        }
    }
    
}

void Free::draw(SDL_Surface* gpScreen) {
    
    SDL_Rect src;
    SDL_Rect dst;
    
    if (mode == FREE_MAIN) {
        
        src.x = 0; src.y = 0; src.w = 320; src.h = 240;
        dst.x = 0; dst.y = 0;
        SDL_BlitSurface(imageOptions, &src, gpScreen, &dst);
        
        //curseurLink
        src.x = 0; src.y = 0; src.w = 16; src.h = 21;
        dst.x = 26; dst.y = 49 + 40 * lineOpt;
        SDL_BlitSurface(curseurLink, &src, gpScreen, &dst);
        
        
        ostringstream oss;
        
        if (gpJoueur->getPrefBg() <= NB_BACKGROUND) {
            if (gpJoueur->getPrefBg() < 10) {
                oss << "0";
            }
            oss << gpJoueur->getPrefBg();
            Text::getInstance()->affiche(gpScreen, oss.str(), 277, 52 + 40 * 2);
            src.x = 0; src.y = 0; src.w = 16; src.h = 16;
            dst.x = 277 - 16 - 8; dst.y = 52 + 40 * 2;
            SDL_BlitSurface(background, &src, gpScreen, &dst);
        } else {
            Text::getInstance()->affiche(gpScreen, " ?", 277, 52 + 40 * 2);
        }
        
        if (gpJoueur->getPrefTh() <= NB_THEME) {
            oss.str("");
            if (gpJoueur->getPrefTh() < 10) {
                oss << "0";
            }
            oss << gpJoueur->getPrefTh();
            Text::getInstance()->affiche(gpScreen, oss.str(), 277, 52 + 40 * 3);
            src.x = 0; src.y = 0; src.w = 14; src.h = 7;
            dst.x = 277 + 1 - 16 - 8; dst.y = 52 + 1 + 4 + 40 * 3;
            SDL_BlitSurface(
                Resources::getInstance()->getTheme(gpJoueur->getPrefTh()), 
                &src, gpScreen, &dst);
        } else {
            Text::getInstance()->affiche(gpScreen, " ?", 277, 52 + 40 * 3);
        }
        
        if (gpJoueur->getPrefMs() <= NB_MUSIC) {
            oss.str("");
            if (gpJoueur->getPrefMs() < 10) {
                oss << "0";
            }
            oss << gpJoueur->getPrefMs();
            Text::getInstance()->affiche(gpScreen, oss.str(), 277, 52 + 40 * 4);
        } else {
            Text::getInstance()->affiche(gpScreen, " ?", 277, 52 + 40 * 4);
        }
        
    } else if (mode == SELECT_PICROSS) {
        
        src.x = 0; src.y = 0; src.w = 320; src.h = 240;
        dst.x = 0; dst.y = 0;
        SDL_BlitSurface(imagePage, &src, gpScreen, &dst);
        
        //curseurLink
        src.x = 0; src.y = 0; src.w = 16; src.h = 21;
        dst.x = 26; dst.y = 49 + 40 * line;
        SDL_BlitSurface(curseurLink, &src, gpScreen, &dst);
        
    } else if (mode == FREE_PICROSS) {
        
        src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
        SDL_BlitSurface(background, &src, gpScreen, &dst);
        gpPicross->draw(gpScreen);
        
        int time;
        if (gpPicross->isVictory()) {
            time = Chrono::getInstance()->getTime();
        }
        else {
            time = Chrono::getInstance()->getCurrentTime();
        }
        
        Text* gpText = Text::getInstance();
        gpText->affiche(gpScreen, gpText->getTime(time), 16, 32);
        
    }
}
