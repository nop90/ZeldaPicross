/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CHRONO_H__
#define __CHRONO_H__

#include "3ds/SDL_3ds.h"

class Chrono {
    public :
        static Chrono* getInstance();
        
        void start();
        void stop();
        void pause();
        void continu();
        void raz();
        int getTime();
        int getCurrentTime();
    private :
        static Chrono instance;
        
        Chrono();
        ~Chrono();
        
        Uint32 startTime;
        int previous;
        int time;
};

#endif  // Chrono.h
