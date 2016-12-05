/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>

#include "Common.h"
#include "Event.h"

class Text {
    public :
        static Text* getInstance();
        
        void draw(SDL_Surface* gpScreen);
        void setText(int idTxt, int vx=8, int vy=152, int vw=304, int vh=80, int cadr=1, bool defil=true, int vit=30);
        void affiche(SDL_Surface* gpScreen, string s, int a, int b);
        void changeId(int i);
        int getId();
        void init();
        void handleEvent(Event* event);
        bool isToDisplay();
        string getText(int i);
        string getTime(int i);
        void setNumber(int n);
    private :
        static Text instance;
        
        Text();
        ~Text();
        
        bool isFinished();
        bool hasNext();
        bool next();
        void chercheText();
        void cutText();
        int wordSize(int deb);
        void displayLetter(SDL_Surface* gpScreen, char c, int vx, int vy);
        
        int vitesse; // millisecondes entre 2 lettres
        int av; //avancement du défilement
        int x;
        int y;
        int w;
        int h;
        int id;
        int idsuiv;
        int number;
        bool def; //si le texte doit défiler
        int cadre; //cadre : 0 = non, 1 = vert, 2 = bleu, 3 = rouge
        string text;
        string buffer;
        bool toDisplay;
        Uint32 lastAnimTime;
        SDL_Surface* imageFont;
        string texts[MAX_TEXTS + 1];
};

#endif  // Text.h
