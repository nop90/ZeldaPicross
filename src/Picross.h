/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PICROSS_H__
#define __PICROSS_H__

#include "Event.h"
#include "Common.h"

class Picross {
    public :
        static Picross* getInstance();
        
        void init(SDL_Surface* pcs, SDL_Surface* thm, PicrossMode = NORMAL);
        void handleEvent(Event* event);
        void loop();
        void draw(SDL_Surface* gpScreen);
        void updateValuesForCreation();
        void setDefeat(bool b);
        void setPause(bool p);
        bool isVictory();
        bool isDefeat();
        bool isPause();
        void useKeyboard();
    private :
        Picross();
        ~Picross();
        static Picross instance;
        
        void drawMouse(SDL_Surface* gpScreen);
        void drawCursor(SDL_Surface* gpScreen, int x1, int y1, int w1, int h1, Uint32 color);
        void drawValue(SDL_Surface* gpScreen, int value, int x1, int y1, int color);
        void drawChiffre(SDL_Surface* gpScreen, int value, int x1, int y1, int color);
        void updateColumnForCreation(int i);
        void updateLineForCreation(int i);
        void fillColumn(int i);
        void fillLine(int i);
        void updateLineColor(int i);
        void updateColumnColor(int i);
        void updateColor();
        void checkVictory();
        void handleMouseEvent(Event* event);
        
        PicrossMode mode;
        bool ready;
        bool victory;
        bool defeat;
        bool pause;
        bool hypothese;
        bool mute;
        int x;
        int y;
        int width;
        int height;
        Uint32 color1;        // first lines
        Uint32 color2;        // second lines
        Uint32 color3;        // cursor
        Uint32 color4;        // cursor start
        Uint32 color5;        // cursor end
        Uint32 color6;        // line pair
        Uint32 color7;        // line unpair
        Uint32 color8;        // current line
        int cursorX;
        int cursorY;
        int cursorStartX;
        int cursorStartY;
        int cursorOldX;
        int cursorOldY;
        CellState states[MAX_COLUMNS][MAX_LINES];
        CellState hidden[MAX_COLUMNS][MAX_LINES];
        int lines[MAX_LINES][MAX_NUMBER_LINES+1];
        int columns[MAX_COLUMNS][MAX_NUMBER_COLUMNS+1];
        int lineColor[MAX_LINES][MAX_NUMBER_LINES];
        int columnColor[MAX_COLUMNS][MAX_NUMBER_COLUMNS];
        SDL_Surface* theme;
        SDL_Surface* chiffres;
        SDL_Surface* mouse;
        int xmin;
        int ymin;
        int xmax;
        int ymax;
        bool useMouse;
        int xmouse;
        int ymouse;
};

#endif  // Picross.h
