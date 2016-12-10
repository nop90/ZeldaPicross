/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <string>
#include <fstream>
#include <iostream>

#include "3ds/SDL_3ds.h"

#include "Text.h"
#include "Common.h"
#include "Resources.h"
#include "Audio.h"
#include "Cadre.h"

Text Text::instance=Text();

Text::Text() : vitesse(40), av(0), x(0), y(0), w(0), h(0), id(0), idsuiv(0), 
number(1), def(false), cadre(0), text(""), buffer(""), toDisplay(false) {
}

Text::~Text() {
}

void Text::init() {
    lastAnimTime = SDL_GetTicks();
    imageFont = Resources::getInstance()->getFont();
    ifstream file("romfs:/texts/texts.txt");
    string line;
    int i = 1;
    while (getline(file, line) && i <= MAX_TEXTS) {
        texts[i++] = line;
    }
    file.close();
}

Text* Text::getInstance() {
    return &instance;
}

void Text::handleEvent(Event* event) {
    if (av == (int)text.length()) {
        if (event->UP) {
            if (id == TEXTS_BEFORE_COMMON + 5
                || id == TEXTS_BEFORE_COMMON + 6
                || id == TEXTS_BEFORE_COMMON + 10) {
                text = texts[--id];
                av = text.length();
                Audio::getInstance()->playSound(3);
            }
        }
        if (event->DOWN) {
            if (id == TEXTS_BEFORE_COMMON + 4
                || id == TEXTS_BEFORE_COMMON + 5
                || id == TEXTS_BEFORE_COMMON + 9) {
                text = texts[++id];
                av = text.length();
                Audio::getInstance()->playSound(3);
            }
        }
    }
    
    if (event->RETURN) {
		next();
		event->TEXT=true;
    }
}

void Text::chercheText() {
    text = texts[id];
}

string Text::getText(int i) {
    return texts[i];
}

string Text::getTime(int i) {
    
    if (i > 359999) { //we can't display more than 99h59m59s
        i = 359999;
    }
    
    int hours = i / 3600;
    int minutes = (i % 3600) / 60;
    int seconds = i % 60;
    ostringstream os;
    if (hours < 10) os << 0;
    os << hours << ":"; 
    if (minutes < 10) os << 0;
    os << minutes << ":";
    if (seconds < 10) os << 0;
    os << seconds;
    return os.str();
}

void Text::affiche(SDL_Surface* gpScreen, std::string s, int a, int b) {
    for (int i = 0; i < (int)s.length(); i++) {
        displayLetter(gpScreen, s.at(i),a,b);
        a+=6;
    }
    //toDisplay = true;
}

void Text::draw(SDL_Surface* gpScreen) {
    
    if (cadre) {
        Cadre::getInstance()->drawCadre(gpScreen, x, y, w, h, cadre - 1);
    }
    
    int a = x+8; int b = y+8;
    for (int i = 0; i < av; i++) {
        displayLetter(gpScreen, text.at(i),a,b);
        a+=6;
        if (a >= x+w-16) {a=x+8; b+=16;}
    }
    
    if(SDL_GetTicks() > lastAnimTime + vitesse && def && av < (int)text.length()) {
        lastAnimTime = SDL_GetTicks();
        do av++;
        while (av < (int)text.length() && text.at(av-1) == ' ');
        if (text.at(av-1) != ' ') Audio::getInstance()->playSound(0,1);
    }
}

bool Text::isFinished() {return (av==(int)text.length());}

int Text::getId() {return id;}

void Text::changeId(int i) {
    id=i; idsuiv=0; text=""; buffer="";
    chercheText();
    cutText();
    if (av>(int)text.length()) av=(int)text.length();
    toDisplay = true;
}

void Text::setText(int idTxt, int vx, int vy, int vw, int vh, int cadr, bool defil, int vit) {
    if (idTxt == 0) return;
    id = idTxt; 
    idsuiv = 0;
    buffer = "";
    chercheText();
    
    x = vx; y = vy; w = vw; h = vh;
    cutText();
        
    def=defil;
    if (def) av = 0;
    else av = text.length();
    
    cadre = cadr;
    
    vitesse = vit;
    
    toDisplay = true;
}

void Text::cutText() {
    //compte le nombre de caractères possibles et largeur et en hauteur
    int nbcol = (w-16)/6 -1;
    int nblig = (h-16)/16;
    int tailleMax = nbcol * nblig;
    int taille;
  
    int k = text.length() - 1;
    while (text.at(k)==' ') {
        text.erase(k--,1);
        if (k < 0) return;
    }
    
    //parcours du texte à afficher; à chaque début de mot, 
    //vérifie que le mot peut tenir sur la ligne
    for (int i = 0; i < (int)text.length(); i++) {
        
        //on ne s'occupe que de la partie à afficher
        if (i >= tailleMax) {
            break;
        }
        
        //supprime les espaces isolés en début de ligne
        if (text.at(i)==' ' && text.at(i+1)!=' ' && i%nbcol == 0) text.erase(i,1);
        //recherche du début du prochain mot
        while(text.at(i)==' ' && i < (int)text.length()-1) i++;
        
        //saute une ligne si trouve une étoile
        if (text.at(i)=='*') {
            text.erase(i,1);
            int nb = (nbcol)-(i%(nbcol));
            for (int j = 0; j < nb; j++) text.insert(i," ");
            continue;
        }
        
        //si le mot dépasse
        taille = wordSize(i);
        if ((i%nbcol)+taille>nbcol) {
            if  (i < tailleMax) {
                //si le mot ne tient pas sur une ligne, on le coupe avec des tirets
                if (taille>nbcol) {
                    text.insert(((i/nbcol)+1)*nbcol-1,"--");
                    i = 1+((i/nbcol)+1)*nbcol;
                }
                //sinon, on ajoute des espaces pour faire commencer le mot à la ligne
                else while((i%nbcol) != 0) {text.insert(i," "); i++;}
            }
        }
        
    }
    
    // si le texte est trop grand, on le coupe en deux
    if ((int)text.length() > tailleMax) {
        buffer = text.substr(tailleMax);
        text = text.substr(0, tailleMax);
    }
}

int Text::wordSize(int deb) {
    int i = deb;
    int total = 0;
    while (text.at(i)!=' ' && text.at(i)!='*') {
        total++; i++; if (i >= (int)text.length()) return total;
    }
    return total;
}

void Text::displayLetter(SDL_Surface* gpScreen, char c, int vx, int vy) {
    SDL_Rect src;
    SDL_Rect dst;
    
    int val = (int)c;
    
    dst.x=vx; dst.y=vy;
    src.h=16;src.w=8;
    
    if(val==13) return;
    if(val==32) return;
    
    // /
    if(val==47) {src.x=52;src.y=151;}
    
    // @ hylien
    if(val==64) {src.x=4;src.y=151;}
            
    // + hylien
    if(val==43) {src.x=20;src.y=151;}
            
    // = hylien
    if(val==61) {src.x=36;src.y=151;}
            
    //minuscules a-z
    if(val>=97 && val<=122) {src.x=4+16*((val-97)%10); src.y=52+16*((val-97)/10);}
            
    //majuscules A-Z
    if(val>=65 && val<=90) {src.x=6+16*((val-65)%10); src.y=2+16*((val-65)/10);}   
    // ç
    if(val==-25) {src.x=148;src.y=34;}
    // é
    if(val==-23) {src.x=100;src.y=84;}
    // ê
    if(val==-22) {src.x=116;src.y=84;}
    // è
    if(val==-24) {src.x=132;src.y=84;}
    // ë
    if(val==-21) {src.x=132;src.y=151;}
    // à
    if(val==-32) {src.x=148;src.y=84;}
    // â
    if(val==-30) {src.x=148;src.y=103;}
    // ä
    if(val==-28) {src.x=148;src.y=135;}
    // î
    if(val==-18) {src.x=84;src.y=119;}
    // ï
    if(val==-17) {src.x=116;src.y=151;}
    // û
    if(val==-5) {src.x=84;src.y=103;}
    // ù
    if(val==-7) {src.x=148;src.y=151;}
    // ü
    if(val==-4) {src.x=116;src.y=135;}
    // ö
    if(val==-10) {src.x=132;src.y=135;}
    // ô
    if(val==-12) {src.x=148;src.y=119;}
            
    //ponctuation
    // -
    if(val==45) {src.x=102;src.y=34;}
    // .
    if(val==46) {src.x=118;src.y=34;}
    // ,
    if(val==44) {src.x=134;src.y=34;}
    // !
    if(val==33) {src.x=3;src.y=135;}
    // ?
    if(val==63) {src.x=19;src.y=135;}
    // (
    if(val==40) {src.x=35;src.y=135;}
    // )
    if(val==41) {src.x=51;src.y=135;}            
    // ' ( avec @ )
    if(val==39) {src.x=67;src.y=135;}
    // :
    if(val==58) {src.x=83;src.y=135;}
    // ... ( avec % )
    if(val==37) {src.x=101;src.y=135;}
    // >
    if(val==62) {src.x=100;src.y=151;}
    // <
    if(val==60) {src.x=84;src.y=151;}
            
    //chiffres            
    if(val>=48 && val<=57) {src.x=3+16*((val-48)%5); src.y=103+16*((val-48)/5);}
    
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
}

bool Text::hasNext() {
    return (buffer != "" || idsuiv > 0 || number > 1);
}

bool Text::next() {
    if (av < (int)text.length()) {
        av = text.length();
        return true;
    }
    if (!hasNext()) {
        Audio::getInstance()->playSound(9);
        toDisplay = false;
        return false;
    }
    if (buffer != "") {
        text = buffer;
        buffer = "";
    }
    else if (idsuiv > 0) {
        id = idsuiv;
        idsuiv = 0;
        chercheText();
    }
    else {
        number--;
        id++;
        chercheText();
    }
    cutText();
    if (def) av = 0;
    else av = text.length();
    Audio::getInstance()->playSound(8);
    return true;
}

bool Text::isToDisplay() {
    return toDisplay;
}

void Text::setNumber(int n) {
    number = n;
}
