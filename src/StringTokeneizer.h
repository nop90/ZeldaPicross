/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STRINGTOKENEIZER_H__
#define __STRINGTOKENEIZER_H__

#include <string>

using namespace std;

class StringTokeneizer {
    public :
        StringTokeneizer(string s, char c = ';');
        ~StringTokeneizer();
        bool hasNext();
        string next();
    private :
        
        string line;
        char carac;
};

#endif  // StringTokeneizer.h
