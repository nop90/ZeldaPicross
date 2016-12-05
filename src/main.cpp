/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <cstdlib>
#include <ctime>
#include <sys/stat.h>

#include "Window.h"

int main(int argc, char** argv) {
    if (argc && argv); //pour éviter un warning.....
    
    std::srand(std::time(NULL));
 
	mkdir("/3ds", 0777);
	mkdir("/3ds/ZeldaPicross", 0777);
	mkdir("/3ds/ZeldaPicross/save", 0777);
	mkdir("/3ds/ZeldaPicross//zpicross", 0777);
   
    Window* window = new Window(true);
    
    window->loop();
    
    delete window;
    
    return 0;
}
