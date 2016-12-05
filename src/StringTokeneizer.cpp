/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "StringTokeneizer.h"

StringTokeneizer::StringTokeneizer(string s, char c) : line(s), carac(c) {
}

StringTokeneizer::~StringTokeneizer() {
}

bool StringTokeneizer::hasNext() {
    return (line.find_first_of(carac) > 0);
}

string StringTokeneizer::next() {
    if (!hasNext()) return "";
    string ret = line.substr(0, line.find_first_of(carac));
    line = line.substr(line.find_first_of(carac) + 1);
    return ret;
}
