/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Window.h"
#include "Keyboard.h"
#include "Event.h"
#include "Game.h"
#include "Audio.h"

SDL_Surface* init(bool* zoom);

Window::Window(bool full) : fullscreen(full), zoom(false) {
    gpScreen = init(&zoom);

    Audio* gpAudio = Audio::getInstance();
    gpAudio->init();

}

Window::~Window() {
    SDL_ShowCursor(SDL_ENABLE);
    SDL_Quit();
}

SDL_Surface* init(bool* zoom) {             // initialise SDL

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1) {
        printf("Could not load SDL : %s\n", SDL_GetError());
        return NULL;
    }
	
    atexit(SDL_Quit);
	
    SDL_WM_SetCaption("Zelda Picross",NULL);
//    SDL_Surface* icon = SDL_LoadBMP("data/images/logos/crayon.ico");
//    SDL_SetColorKey(icon,SDL_SRCCOLORKEY,SDL_MapRGB(icon->format,0,0,0));
//    SDL_WM_SetIcon(icon,NULL);

    SDL_ShowCursor(SDL_DISABLE);

    SDL_Rect** modes;
    int gBpp = 1;//0;

/*    
    modes = SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
    if (modes == (SDL_Rect**)-1) gBpp = 1;
    else {
        for (int i = 0; modes[i]; ++i) {
            if (modes[i]->w == 320 && modes[i]->h == 240) {
                gBpp = 1; break;
            }
        }
    }
*/    
/*    if(!gBpp) {
        *zoom = true;
        return SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    } else {
*/
        return SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_TOPSCREEN);
//    }
}

void Window::loop() {
    
    if (gpScreen == NULL) {
        return;
    }
   
    SDL_Rect src;
    SDL_Rect dst;
    src.w=640; src.h=480; src.y=0;src.x=0;dst.x=0; dst.y=0;
    
    SDL_Surface* gpScreen2 = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    SDL_Surface* gpScreen3 = NULL;
    
    Event* event;
    
    bool gLoop = true;
    
    Uint32 lastAnimTime = SDL_GetTicks();
    
    Keyboard* gpKeyboard = new Keyboard();
    Game* gpGame = Game::getInstance();
    gpGame->init();
    
    while (gLoop) {
        
        
        gpKeyboard->pollEvent();
        event = gpKeyboard->getEvent();
        
        gLoop = handleEvent(event, gpGame);
        
        gpGame->loop();
        
        gpGame->draw(gpScreen2);
        
        
  /*
      if (zoom || !fullscreen) {
            SDL_FreeSurface(gpScreen3);
            gpScreen3 = zoomSurface(gpScreen2, 2, 2, 0);
            SDL_BlitSurface(gpScreen3, &src, gpScreen, &dst);
        } else {
*/
            SDL_BlitSurface(gpScreen2, &src, gpScreen, &dst);
//        }
        
        SDL_Flip(gpScreen);
        
        if (SDL_GetTicks() < lastAnimTime + 20) {
            SDL_Delay(lastAnimTime+20-SDL_GetTicks());
        }
        lastAnimTime = SDL_GetTicks();
    
    }
    
    Audio::getInstance()->stopMusic();
    
    delete gpKeyboard;
    
    SDL_FreeSurface(gpScreen2);
    SDL_FreeSurface(gpScreen3);
    
}

bool Window::handleEvent(Event* event, Game* gpGame) {
    if (event->FULLSCREEN) {
        event->FULLSCREEN = false;
        toggleFullScreen();
        return true;
    }
    gpGame->handleEvent(event);
    return (!event->QUIT);
}

void Window::toggleFullScreen() {
    int gFullScreen = (fullscreen ? 0 : SDL_FULLSCREEN);
    gFullScreen ? SDL_ShowCursor(SDL_DISABLE) : SDL_ShowCursor(SDL_ENABLE);
/*
    if (zoom || gFullScreen == 0) {
        gpScreen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE
                                    |SDL_DOUBLEBUF|gFullScreen);
    } else {
*/
        gpScreen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE
                                    |SDL_DOUBLEBUF|gFullScreen);
//    }
    fullscreen = (gFullScreen != 0);
}

    
