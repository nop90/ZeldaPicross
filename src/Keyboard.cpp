/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "3ds/SDL_3ds.h"

#include "Keyboard.h"
#include "Common.h"

Keyboard::Keyboard() : tmpDirUp(0), tmpDirDown(0), tmpDirLeft(0), tmpDirRight(0), 
tmpAction(0), tmpReturn(0), tmpHypo(0), tmpMouse(0), tmpEscap(0), tmpMusic(0), 
startTime(0), running(false) {
    event = new Event();
}

Keyboard::~Keyboard() {
    delete event;
}

void Keyboard::pollEvent() {
    
    event->MOUSE_ACTION = false;
    event->MOUSE_FLAG = false;
    event->MOUSE_CANCEL_ACTION = false;
    event->MOUSE_CANCEL_FLAG = false;
    
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT : 
                event->QUIT=true;
                return;
            case SDL_KEYDOWN :
                pollKey(sdlEvent);
                break;
            case SDL_MOUSEBUTTONDOWN :
                if (sdlEvent.button.button == SDL_BUTTON_LEFT 
                && !event->MOUSE_HOLD_ACTION) {
                    if (event->MOUSE_HOLD_FLAG) {
                        event->MOUSE_HOLD_FLAG = false;
                        event->MOUSE_CANCEL_FLAG = true;
                    } else {
                        event->MOUSE_HOLD_ACTION = true;
                    }
                } else if (sdlEvent.button.button == SDL_BUTTON_RIGHT 
                && !event->MOUSE_HOLD_FLAG) {
                    if (event->MOUSE_HOLD_ACTION) {
                        event->MOUSE_HOLD_ACTION = false;
                        event->MOUSE_CANCEL_ACTION = true;
                    } else {
                        event->MOUSE_HOLD_FLAG = true;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP :
                if (event->MOUSE_HOLD_ACTION && !event->MOUSE_HOLD_FLAG) {
                    event->MOUSE_HOLD_ACTION = false;
                    event->MOUSE_ACTION = true;
                } else if (event->MOUSE_HOLD_FLAG && !event->MOUSE_HOLD_ACTION) {
                    event->MOUSE_HOLD_FLAG = false;
                    event->MOUSE_FLAG = true;
                }
                break;
        }
    }
//    Uint8* keys = SDL_GetKeyState(NULL);
    int keys = SDL_GetKeyState(NULL);
    pollKeys(keys);
    
}

void Keyboard::pollKey(SDL_Event sdlEvent) {
    event->QUIT = false;
    event->ESCAPE = false;
    event->FULLSCREEN = false;
    switch (sdlEvent.key.keysym.sym) {
        case SDLK_ESCAPE :
            event->QUIT=true;
            event->ESCAPE=true;
            break;
        case SDLK_F4 :
            if (sdlEvent.key.keysym.mod & KMOD_ALT) event->QUIT=true;
            break;
        case SDLK_RETURN :
            if (sdlEvent.key.keysym.mod & KMOD_CTRL) event->FULLSCREEN=true;
            break;
        default : break;
    }
}

void Keyboard::pollKeys(int keys) {
    
    
    if (keys & SDLK_UP) {
        if (tmpDirUp == 0) {
            event->UP = true;
            tmpDirUp = 1;
            currentDir = UP;
            running = false;
            startTime = SDL_GetTicks();
        } else {
            if (currentDir == UP) {
                if (running) {
                    if ((SDL_GetTicks() - startTime) > TIME_WHILE_RUNNING) {
                        startTime = SDL_GetTicks();
                        event->UP = true;
                    } else {
                        event->UP = false;
                    }
                } else {
                    if ((SDL_GetTicks() - startTime) > TIME_BEFORE_RUNNING) {
                        startTime = SDL_GetTicks();
                        running = true;
                        event->UP = true;
                    } else {
                        event->UP = false;
                    }
                }
            } else {
                event->UP = false;
            }
        }
    } else {
        tmpDirUp = 0;
        event->UP = false;
    }
    
    if (keys&SDLK_DOWN) {
        if (tmpDirDown == 0) {
            event->DOWN = true;
            tmpDirDown = 1;
            currentDir = DOWN;
            running = false;
            startTime = SDL_GetTicks();
        } else {
            if (currentDir == DOWN) {
                if (running) {
                    if ((SDL_GetTicks() - startTime) > TIME_WHILE_RUNNING) {
                        startTime = SDL_GetTicks();
                        event->DOWN = true;
                    } else {
                        event->DOWN = false;
                    }
                } else {
                    if ((SDL_GetTicks() - startTime) > TIME_BEFORE_RUNNING) {
                        startTime = SDL_GetTicks();
                        running = true;
                        event->DOWN = true;
                    } else {
                        event->DOWN = false;
                    }
                }
            } else {
                event->DOWN = false;
            }
        }
    } else {
        tmpDirDown = 0;
        event->DOWN = false;
    }
    
    if (keys&SDLK_LEFT) {
        if (tmpDirLeft == 0) {
            event->LEFT = true;
            tmpDirLeft = 1;
            currentDir = LEFT;
            running = false;
            startTime = SDL_GetTicks();
        } else {
            if (currentDir == LEFT) {
                if (running) {
                    if ((SDL_GetTicks() - startTime) > TIME_WHILE_RUNNING) {
                        startTime = SDL_GetTicks();
                        event->LEFT = true;
                    } else {
                        event->LEFT = false;
                    }
                } else {
                    if ((SDL_GetTicks() - startTime) > TIME_BEFORE_RUNNING) {
                        startTime = SDL_GetTicks();
                        running = true;
                        event->LEFT = true;
                    } else {
                        event->LEFT = false;
                    }
                }
            } else {
                event->LEFT = false;
            }
        }
    } else {
        tmpDirLeft = 0;
        event->LEFT = false;
    }
    
    if (keys&SDLK_RIGHT) {
        if (tmpDirRight == 0) {
            event->RIGHT = true;
            tmpDirRight = 1;
            currentDir = RIGHT;
            running = false;
            startTime = SDL_GetTicks();
        } else {
            if (currentDir == RIGHT) {
                if (running) {
                    if ((SDL_GetTicks() - startTime) > TIME_WHILE_RUNNING) {
                        startTime = SDL_GetTicks();
                        event->RIGHT = true;
                    } else {
                        event->RIGHT = false;
                    }
                } else {
                    if ((SDL_GetTicks() - startTime) > TIME_BEFORE_RUNNING) {
                        startTime = SDL_GetTicks();
                        running = true;
                        event->RIGHT = true;
                    } else {
                        event->RIGHT = false;
                    }
                }
            } else {
                event->RIGHT = false;
            }
        }
    } else {
        tmpDirRight = 0;
        event->RIGHT = false;
    }
    
    
    event->ACTION = false;
    if ((keys&SDLK_w || keys&SDLK_z) && !event->HOLD_ACTION  
    && !event->HOLD_FLAG && !tmpAction) {
        event->HOLD_ACTION = true;
        tmpAction = 1;
    }
    if (!(keys&SDLK_w) && !(keys&SDLK_z) && event->HOLD_ACTION) {
        event->HOLD_ACTION = false;
        event->ACTION = true;
        tmpAction = 0;
    }
    
    event->FLAG = false;
    if (keys&SDLK_x && !event->HOLD_ACTION  && !event->HOLD_FLAG 
    && !tmpAction) {
        event->HOLD_FLAG = true;
        tmpAction = 1;
    }
    if (!(keys&SDLK_x) && event->HOLD_FLAG) {
        event->HOLD_FLAG = false;
        event->FLAG = true;
        tmpAction = 0;
    }
    
    // cancel action
    if ((keys&SDLK_x) && event->HOLD_ACTION  && !event->HOLD_FLAG) {
        event->HOLD_ACTION = false;
        event->CANCEL_ACTION = true;
        tmpAction = 1;
    }
    if (!(keys&SDLK_x) && !(keys&SDLK_w) && !(keys&SDLK_z) 
    && event->CANCEL_ACTION) {
        event->CANCEL_ACTION = false;
        tmpAction = 0;
    }
    
    // cancel flag
    if ((keys&SDLK_w || keys&SDLK_z) && !event->HOLD_ACTION  
    && event->HOLD_FLAG) {
        event->HOLD_FLAG = false;
        event->CANCEL_FLAG = true;
        tmpAction = 1;
    }
    if (!(keys&SDLK_x) && !(keys&SDLK_w) && !(keys&SDLK_z) && event->CANCEL_FLAG) {
        event->CANCEL_FLAG = false;
        tmpAction = 0;
    }
    
    
    event->RETURN = false;
    if ((keys&SDLK_RETURN || keys&SDLK_KP_ENTER) && !tmpReturn) {
        event->RETURN = true;
        tmpReturn = 1;
    }
    if (!(keys&SDLK_RETURN) && !(keys&SDLK_KP_ENTER) && tmpReturn) {
        tmpReturn = 0;
    }
    
    event->HYPOTHESE = false;
    if (keys&SDLK_h && !event->HYPOTHESE && !tmpHypo) {
        event->HYPOTHESE = true;
        tmpHypo = 1;
    }
    if (!(keys&SDLK_h) && tmpHypo) {
        tmpHypo = 0;
    }
    
    event->MOUSE_ON_OFF = false;
    if (keys&SDLK_TAB && !event->MOUSE_ON_OFF && !tmpMouse) {
        event->MOUSE_ON_OFF = true;
        tmpMouse = 1;
    }
    if (!(keys&SDLK_TAB) && tmpMouse) {
        tmpMouse = 0;
    }
    
    
    event->MUSIC_ON_OFF = false;
    if ((keys&SDLK_m || keys&SDLK_SEMICOLON) && !event->MUSIC_ON_OFF && !tmpMusic) {
        event->MUSIC_ON_OFF = true;
        tmpMusic = 1;
    }
    if (!(keys&SDLK_m) && !(keys&SDLK_SEMICOLON) && tmpMusic) {
        tmpMusic = 0;
    }
    
    
    /*if ((keys[SDLK_w] || keys[SDLK_z]) && tmpAction == 0) {
        event->ACTION = true;
        tmpAction = 1;
    } else {
        event->ACTION = false;
    }
    if (!keys[SDLK_w] && !keys[SDLK_z]) {
        tmpAction = 0;
    }*/
    
}

Event* Keyboard::getEvent() {
    return event;
}

