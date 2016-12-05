/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Common.h"

class Resources {
    public:
        static Resources* getInstance();
        
        SDL_Surface* getTheme(int i);
        SDL_Surface* getBackground(int i);
        SDL_Surface* getPicrossBG(int i);
        SDL_Surface* getPicross(int i);
        SDL_Surface* getMap(int i);
        SDL_Surface* getIcon(int i);
        SDL_Surface* getChiffres();
        SDL_Surface* getFont();
        SDL_Surface* getLink();
        SDL_Surface* getStatus();
        SDL_Surface* getCadre();
        SDL_Surface* getBoss();
        SDL_Surface* getLogo();
        SDL_Surface* getTitreBG();
        SDL_Surface* getTitreLogo();
        SDL_Surface* getCadreMenu();
        SDL_Surface* getLinkMenu();
        SDL_Surface* getFeeMenu();
        SDL_Surface* getMouse();
        SDL_Surface* getStep(int i);
        SDL_Surface* getIntroFond1();
        SDL_Surface* getIntroFond2();
        SDL_Surface* getEndingFond();
        SDL_Surface* getTreeFond();
        
    private:
        SDL_Surface* convertImage(SDL_Surface* im);
        
        Resources();
        ~Resources();
        static Resources instance;
        
        SDL_Surface* theme[10];
        SDL_Surface* background[15];
        SDL_Surface* picrossBG[15];
        SDL_Surface* map[10];
        SDL_Surface* icon[10];
        SDL_Surface* picross[NB_PICROSS];
        SDL_Surface* chiffres;
        SDL_Surface* font;
        SDL_Surface* link;
        SDL_Surface* status;
        SDL_Surface* cadre;
        SDL_Surface* boss;
        SDL_Surface* logo;
        SDL_Surface* titreBG;
        SDL_Surface* titreLogo;
        SDL_Surface* cadreMenu;
        SDL_Surface* linkMenu;
        SDL_Surface* feeMenu;
        SDL_Surface* mouse;
        SDL_Surface* step[3];
        SDL_Surface* introFond1;
        SDL_Surface* introFond2;
        SDL_Surface* endingFond;
        SDL_Surface* treeFond;
};

#endif  // Resources.h
