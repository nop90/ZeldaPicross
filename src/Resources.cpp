/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Common.h"
#include "Resources.h"

Resources Resources::instance=Resources();

Resources::Resources() {
    for (int i = 0; i < 10; i++) {;
        theme[i] = 0;
        map[i] = 0;
        icon[i] = 0;
    }
    for (int i = 0; i < 15; i++) {
        background[i] = 0;
        picrossBG[i] = 0;
    }
    for (int i = 0; i < NB_PICROSS; i++) {
        picross[i] = 0;
    }
    chiffres = 0;
    font = 0;
    link = 0;
    status = 0;
    cadre = 0;
    boss = 0;
    logo = 0;
    titreBG = 0;
    titreLogo = 0;
    linkMenu = 0;
    feeMenu = 0;
    mouse = 0;
    for (int i = 0; i < 3; i++) {
        step[i] = 0;
    }
    introFond1 = 0;
    introFond2 = 0;
    endingFond = 0;
    treeFond = 0;
}

Resources::~Resources() {
    for (int i = 0; i < 10; i++) {
        SDL_FreeSurface(theme[i]);
        SDL_FreeSurface(map[i]);
        SDL_FreeSurface(icon[i]);
    }
    for (int i = 0; i < 15; i++) {
        SDL_FreeSurface(background[i]);
        SDL_FreeSurface(picrossBG[i]);
    }
    for (int i = 0; i < NB_PICROSS; i++) {
        SDL_FreeSurface(picross[i]);
    }
    SDL_FreeSurface(chiffres);
    SDL_FreeSurface(font);
    SDL_FreeSurface(link);
    SDL_FreeSurface(status);
    SDL_FreeSurface(cadre);
    SDL_FreeSurface(boss);
    SDL_FreeSurface(logo);
    SDL_FreeSurface(titreBG);
    SDL_FreeSurface(titreLogo);
    SDL_FreeSurface(mouse);
    for (int i = 0; i < 3; i++) {
        SDL_FreeSurface(step[i]);
    }
    SDL_FreeSurface(introFond1);
    SDL_FreeSurface(introFond2);
    SDL_FreeSurface(endingFond);
    SDL_FreeSurface(treeFond);
}

Resources* Resources::getInstance() {
    return &instance;
}

SDL_Surface* Resources::convertImage(SDL_Surface* tmp) {
    if (tmp == NULL) return NULL;
 //   SDL_Surface* im = SDL_DisplayFormat(tmp);
 //   SDL_FreeSurface(tmp);
 //   return im;

    return tmp;
}

SDL_Surface* Resources::getTheme(int i) {
    if (i == 0 || i > 10) {
        return 0;
    }
    if (theme[i-1] == 0) {
        ostringstream im;
        im << i;
        theme[i-1] = IMG_Load(
            ("romfs:/images/theme/theme" + im.str() + ".png").c_str());
        theme[i-1] = convertImage(theme[i-1]);
    }
    return theme[i-1];
}

SDL_Surface* Resources::getChiffres() {
    if (chiffres == 0) {
        chiffres = IMG_Load("romfs:/images/font/chiffres.png");
        if (chiffres) {
            SDL_SetColorKey(chiffres,
                SDL_SRCCOLORKEY,SDL_MapRGB(chiffres->format,0,0,255));
        }
        chiffres = convertImage(chiffres);
    }
    return chiffres;
}

SDL_Surface* Resources::getBackground(int i) {
    if (i < 0 || i > 10) {
        return 0;
    }
    if (background[i] == 0) {
        ostringstream im;
        im << i;
        background[i] = IMG_Load(
            ("romfs:/images/background/background" + im.str() + ".png").c_str());
        background[i] = convertImage(background[i]);
    }
    return background[i];
}

SDL_Surface* Resources::getPicross(int i) {
    if (i == 0 || i > NB_PICROSS) {
        return 0;
    }
    if (picross[i-1] == 0) {
        ostringstream im;
        im << i;
        picross[i-1] = IMG_Load(
            ("romfs:/images/picross/picross" + im.str() + ".png").c_str());
        picross[i-1] = convertImage(picross[i-1]);
    }
    return picross[i-1];
}

SDL_Surface* Resources::getMap(int i) {
    if (i == 0 || i > 10) {
        return 0;
    }
    if (map[i-1] == 0) {
        ostringstream im;
        im << i;
        map[i-1] = IMG_Load(
            ("romfs:/images/map/map" + im.str() + ".png").c_str());
        if (map[i-1]) {
            SDL_SetColorKey(map[i-1],
                SDL_SRCCOLORKEY,SDL_MapRGB(map[i-1]->format,0,0,255));
        }
        map[i-1] = convertImage(map[i-1]);
    }
    return map[i-1];
}

SDL_Surface* Resources::getIcon(int i) {
    if (i < 0 || i > 10) {
        return 0;
    }
    if (icon[i] == 0) {
        ostringstream im;
        im << i;
        icon[i] = IMG_Load(
            ("romfs:/images/icon/icon" + im.str() + ".png").c_str());
        if (icon[i]) {
            SDL_SetColorKey(icon[i],
                SDL_SRCCOLORKEY,SDL_MapRGB(icon[i]->format,0,0,255));
        }
        icon[i] = convertImage(icon[i]);
    }
    return icon[i];
}

SDL_Surface* Resources::getPicrossBG(int i) {
    if (i == 0 || i > 15) {
        return 0;
    }
    if (picrossBG[i-1] == 0) {
        ostringstream im;
        im << i;
        picrossBG[i-1] = IMG_Load(
            ("romfs:/images/background/picross" + im.str() + ".png").c_str());
        picrossBG[i-1] = convertImage(picrossBG[i-1]);
    }
    return picrossBG[i-1];
}

SDL_Surface* Resources::getFont() {
    if (font == 0) {
        font = IMG_Load("romfs:/images/font/font.png");
        if (font) {
            SDL_SetColorKey(font,SDL_SRCCOLORKEY,
                SDL_MapRGB(font->format,0,0,255));
        }
        font = convertImage(font);
    }
    return font;
}

SDL_Surface* Resources::getLink() {
    if (link == 0) {
        link = IMG_Load("romfs:/images/link/link.png");
        if (link) {
            SDL_SetColorKey(link,SDL_SRCCOLORKEY,
                SDL_MapRGB(link->format,0,0,255));
        }
        link = convertImage(link);
    }
    return link;
}

SDL_Surface* Resources::getStatus() {
    if (status == 0) {
        status = IMG_Load("romfs:/images/icon/objets.png");
        if (status) {
            SDL_SetColorKey(status,
                SDL_SRCCOLORKEY,SDL_MapRGB(status->format,0,0,255));
        }
        status = convertImage(status);
    }
    return status;
}

SDL_Surface* Resources::getCadre() {
    if (cadre == 0) {
        cadre = IMG_Load("romfs:/images/menu/bord.png");
        if (cadre) {
            SDL_SetColorKey(cadre,
                SDL_SRCCOLORKEY,SDL_MapRGB(cadre->format,0,0,255));
        }
        cadre = convertImage(cadre);
    }
    return cadre;
}

SDL_Surface* Resources::getBoss() {
    if (boss == 0) {
        boss = IMG_Load("romfs:/images/map/boss.png");
        if (boss) {
            SDL_SetColorKey(boss,
                SDL_SRCCOLORKEY,SDL_MapRGB(boss->format,0,0,255));
        }
        boss = convertImage(boss);
    }
    return boss;
}

SDL_Surface* Resources::getLogo() {
    if (logo == 0) {
        logo = IMG_Load("romfs:/images/logos/logo.png");
        logo = convertImage(logo);
    }
    return logo;
}

SDL_Surface* Resources::getTitreBG() {
    if (titreBG == 0) {
        titreBG = IMG_Load("romfs:/images/logos/fond.png");
        titreBG = convertImage(titreBG);
    }
    return titreBG;
}

SDL_Surface* Resources::getTitreLogo() {
    if (titreLogo == 0) {
        titreLogo = IMG_Load("romfs:/images/logos/titre.png");
        if (titreLogo) {
            SDL_SetColorKey(titreLogo,
                SDL_SRCCOLORKEY,SDL_MapRGB(titreLogo->format,0,0,255));
        }
        titreLogo = convertImage(titreLogo);
    }
    return titreLogo;
}

SDL_Surface* Resources::getCadreMenu() {
    if (cadreMenu == 0) {
        cadreMenu = IMG_Load("romfs:/images/menu/cadres.png");
        if (cadreMenu) {
            SDL_SetColorKey(cadreMenu,
                SDL_SRCCOLORKEY,SDL_MapRGB(cadreMenu->format,0,0,255));
        }
        cadreMenu = convertImage(cadreMenu);
    }
    return cadreMenu;
}

SDL_Surface* Resources::getLinkMenu() {
    if (linkMenu == 0) {
        linkMenu = IMG_Load("romfs:/images/menu/curseur.png");
        if (linkMenu) {
            SDL_SetColorKey(linkMenu,
                SDL_SRCCOLORKEY,SDL_MapRGB(linkMenu->format,0,0,255));
        }
        linkMenu = convertImage(linkMenu);
    }
    return linkMenu;
}

SDL_Surface* Resources::getFeeMenu() {
    if (feeMenu == 0) {
        feeMenu = IMG_Load("romfs:/images/menu/fee.png");
        feeMenu = convertImage(feeMenu);
    }
    return feeMenu;
}

SDL_Surface* Resources::getMouse() {
    if (mouse == 0) {
        mouse = IMG_Load("romfs:/images/icon/mouse.png");
        if (mouse) {
            SDL_SetColorKey(mouse,
                SDL_SRCCOLORKEY,SDL_MapRGB(mouse->format,0,0,255));
        }
        mouse = convertImage(mouse);
    }
    return mouse;
}

SDL_Surface* Resources::getStep(int i) {
    if (i < 0 || i > 2) {
        return 0;
    }
    if (step[i] == 0) {
        ostringstream im;
        im << (i + 1);
        step[i] = IMG_Load(
            ("romfs:/images/logos/intro" + im.str() + ".png").c_str());
        step[i] = convertImage(step[i]);
    }
    return step[i];
}

SDL_Surface* Resources::getIntroFond1() {
    if (introFond1 == 0) {
        introFond1 = IMG_Load("romfs:/images/logos/fond1.png");
        if (introFond1) {
            SDL_SetColorKey(introFond1,
                SDL_SRCCOLORKEY,SDL_MapRGB(introFond1->format,0,0,255));
        }
        introFond1 = convertImage(introFond1);
    }
    return introFond1;
}

SDL_Surface* Resources::getIntroFond2() {
    if (introFond2 == 0) {
        introFond2 = IMG_Load("romfs:/images/logos/fond2.png");
        introFond2 = convertImage(introFond2);
    }
    return introFond2;
}

SDL_Surface* Resources::getEndingFond() {
    if (endingFond == 0) {
        endingFond = IMG_Load("romfs:/images/logos/fin.png");
        endingFond = convertImage(endingFond);
    }
    return endingFond;
}

SDL_Surface* Resources::getTreeFond() {
    if (treeFond == 0) {
        treeFond = IMG_Load("romfs:/images/logos/arbre.png");
        treeFond = convertImage(treeFond);
    }
    return treeFond;
}

