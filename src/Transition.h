/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "Common.h"

class Transition {
    public :
        static Transition* getInstance();
        
        void loop();
        void draw(SDL_Surface* gpScreen);
        void init(TransitionMode m = CIRCLE);
        void begin(int x = 160, int y = 120); // step : IN -> WAIT
        void next(int x = 160, int y = 120); // step : OUT -> END
        bool isToDisplay();
        TransitionStep getStep();
        bool isFinalRound();
    private :
        static Transition instance;
        
        Transition();
        ~Transition();
        
        TransitionMode mode;
        TransitionStep step;
        int anim;
        int animMax;
        bool toDisplay;
        int x;
        int y;
        //Uint32 lastAnimTime;
        SDL_Surface* image;
};

#endif  // Transition.h
