/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Event.h"
#include "Common.h"
#include "Picross.h"
#include "Resources.h"
#include "Audio.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y) {
	return GetPixel32(surface, x, y);
	
/*
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve 
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:        
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;       // shouldn't happen, but avoids warnings
    }
*/
}

Picross Picross::instance=Picross();

Picross* Picross::getInstance() {
    return &instance;
}

Picross::Picross() : ready(false) {
    useMouse = false;
}

Picross::~Picross() {
}

void Picross::init(SDL_Surface* pcs, SDL_Surface* thm, PicrossMode pMode) {
    
    mouse = Resources::getInstance()->getMouse();

    mode = pMode;
    
    x = PICROSS_X;
    y = PICROSS_Y;
    width = pcs->w;
    height = pcs->h;
    
    xmin = x; xmax = x + width * 8 - 1; ymin = y; ymax = y + height * 8 - 1;
    xmouse = xmin + ((xmax - xmin) / 2);
    ymouse = ymin + ((ymax - ymin) / 2);
    //SDL_WarpMouse(xmouse, ymouse);
    
    cursorX = useMouse ? width / 2 : 0;
    cursorY = useMouse ? height / 2 : 0;
    cursorStartX = -1;
    cursorStartY = -1;
    cursorOldX = -1;
    cursorOldY = -1;
    
    for (int i = 0; i < MAX_COLUMNS; i++) {
        for (int j = 0; j < MAX_LINES; j++) {
            states[i][j] = EMPTY;
            if (i >= width || j >= height) {
                hidden[i][j] = EMPTY;
            } else {
                hidden[i][j] = ((getPixel(pcs, i, j)&0xffffff) == 0 ? OK : KO);
            }
        }
    }
    for (int i = 0; i < MAX_LINES; i++) {
        lines[i][0] = 1;
        lines[i][1] = 0;
    }
    for (int i = 0; i < MAX_COLUMNS; i++) {
        columns[i][0] = 1;
        columns[i][1] = 0;
    }
    
    theme = thm;
    color1 = getPixel(theme, 21, 0);
    color2 = getPixel(theme, 22, 0);
    color3 = getPixel(theme, 23, 0);
    color4 = getPixel(theme, 24, 0);
    color5 = getPixel(theme, 25, 0);
    color6 = getPixel(theme, 26, 0);
    color7 = getPixel(theme, 27, 0);
    color8 = getPixel(theme, 28, 0);
    chiffres = Resources::getInstance()->getChiffres();
    
    for (int i = 0 ; i < width; i++) {
        fillColumn(i);
        updateColumnColor(i); // if there is a 0
    }
    for (int j = 0 ; j < height; j++) {
        fillLine(j);
        updateLineColor(j); // if there is a 0
    }
    
    victory = false;
    defeat = false;
    pause = false;
    hypothese = false;
    mute = false;
    
    ready = true;
}

void Picross::checkVictory() {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if ((hidden[i][j] == OK && states[i][j] != OK)
                || (hidden[i][j] == KO && states[i][j] == OK)) {
                victory = false;
                return;
            }
        }
    }
    victory = true;
    return;
}

bool Picross::isVictory() {
    if (!ready) {
        return false;
    }
    return victory;
}

bool Picross::isDefeat() {
    if (!ready) {
        return false;
    }
    return defeat;
}

bool Picross::isPause() {
    if (!ready) {
        return false;
    }
    return pause;
}

void Picross::setDefeat(bool d) {defeat = d;}

void Picross::setPause(bool p) {pause = p;}

void Picross::updateColor() {
    int startX;
    int startY;
    int stopX;
    int stopY;
    if (cursorOldX > -1 && cursorOldY > -1) {
        startX = (cursorX < cursorOldX ? cursorX : cursorOldX);
        startY = (cursorY < cursorOldY ? cursorY : cursorOldY);
        stopX = (cursorX > cursorOldX ? cursorX : cursorOldX);
        stopY = (cursorY > cursorOldY ? cursorY : cursorOldY);
    } else {
        startX = stopX = cursorX;
        startY = stopY = cursorY;
    }
    for (int i = startX ; i <= stopX; i++) {
        updateColumnColor(i);
    }
    for (int j = startY ; j <= stopY; j++) {
        updateLineColor(j);
    }
    
}

void Picross::updateColumnColor(int i) {
    int nbValue = columns[i][0];
    
    // init to 0
    for (int k = 0; k < nbValue; k++) {
        columnColor[i][k] = 0;
    }
    
    // check if all is okay
    int total = 0;
    int nb = 0;
    int nbmax = 0;
    bool flagBetweenAll = true;
    bool flagFromLast = true;
    int column[MAX_NUMBER_COLUMNS+1];
    for (int j = 0; j < height; j++) {
        if (states[i][j] == OK || states[i][j] == HYP || states[i][j] == H_OK) {
            if (nb == 0 && !flagFromLast) {
                flagBetweenAll = false;
            }
            flagFromLast = false;
            nb++;
            if (j == height - 1) {
                column[++total] = nb;
                if (nbmax < nb) nbmax=nb;
                nb = 0;
            } else if (states[i][j+1] != OK && states[i][j+1] != HYP 
            && states[i][j+1] != H_OK) {
                column[++total] = nb;
                if (nbmax < nb) nbmax=nb;
                nb = 0;
            }
        }
        if (states[i][j] == KO || states[i][j] == H_KO) {
            flagFromLast = true;
        }
    }
    if (total > 0) {
        column[0] = total;
    } else {
        total = 1;
        column[0] = 1;
        column[1] = 0;
    }
    if (total == nbValue) {
        bool isOk = true;
        for (int j = 0; j < nbValue; j++) {
            if (column[j+1] != columns[i][j+1]) {
                isOk = false;
                break;
            }
        }
        if (isOk) {
            for (int j = 0; j < nbValue; j++) {
                columnColor[i][j] = 1;
            }
            return;
        }
    } else if (total > nbValue && flagBetweenAll) { // error -> too many values
        for (int k = 0; k < nbValue; k++) {
            columnColor[i][k] = 2;
        }
        return;
    }
    // check max values
    int max = 0;
    for (int j = 0; j < nbValue; j++) {
        int value = columns[i][j+1];
        if (max < value) max=value;
    }
    if (nbmax > max) {// error
        for (int k = 0; k < nbValue; k++) {
            columnColor[i][k] = 2;
        }
        return;
    }
    
    
    
    // check by the up
    total = 0;
    nb = 0;
    for (int j = 0; j < height; j++) {
        switch (states[i][j]) {
            case EMPTY : j = height; break;
            case OK :
            case HYP :
            case H_OK :
                nb++;
                if (nb == 1 && total == nbValue) {
                    for (int k = 0; k < nbValue; k++) {
                        columnColor[i][k] = 2;
                    }
                    return;
                }
                if (j + 1 < height) {
                    break;
                }
            case KO :
            case H_KO :
                if (nb > 0) {
                    if (total < nbValue) {
                        if (nb == columns[i][total+1]) {
                            columnColor[i][total] = 1;
                            nb = 0;
                            total++;
                        } else { // error
                            for (int k = 0; k < nbValue; k++) {
                                columnColor[i][k] = 2;
                            }
                            return;
                        }
                    } else { // error
                        for (int k = 0; k < nbValue; k++) {
                            columnColor[i][k] = 2;
                        }
                        return;
                    }
                }
                if (j + 1 == height && total < nbValue) { // error
                    for (int k = 0; k < nbValue; k++) {
                        columnColor[i][k] = 2;
                    }
                    return;
                }
                break;
        }
    }
    
    // check by the down
    total = 0;
    nb = 0;
    for (int j = height - 1; j >= 0; j--) {
        switch (states[i][j]) {
            case EMPTY : j = -1; break;
            case OK :
            case HYP :
            case H_OK :
                nb++;
                if (nb == 1 && total == nbValue) {
                    for (int k = 0; k < nbValue; k++) {
                        columnColor[i][k] = 2;
                    }
                    return;
                }
                if (j > 0) {
                    break;
                }
            case KO :
            case H_KO :
                if (nb > 0) {
                    if (total < nbValue) {
                        if (nb == columns[i][nbValue - total]) {
                            columnColor[i][nbValue - total - 1] = 1;
                            nb = 0;
                            total++;
                        } else { // error
                            for (int k = 0; k < nbValue; k++) {
                                columnColor[i][k] = 2;
                            }
                            return;
                        }
                    } else { // error
                        for (int k = 0; k < nbValue; k++) {
                            columnColor[i][k] = 2;
                        }
                        return;
                    }
                }
                // shouldn't happen
                if (j == 0 && total < nbValue) { // error
                    for (int k = 0; k < nbValue; k++) {
                        columnColor[i][k] = 2;
                    }
                    return;
                }
                break;
        }
    }
    
    
    
    
}

void Picross::updateLineColor(int j) {
    int nbValue = lines[j][0];
    
    // init to 0
    for (int k = 0; k < nbValue; k++) {
        lineColor[j][k] = 0;
    }
    
    // check if all is okay
    int total = 0;
    int nb = 0;
    int nbmax = 0;
    bool flagBetweenAll = true;
    bool flagFromLast = true;
    int line[MAX_NUMBER_LINES+1];
    for (int i = 0; i < width; i++) {
        if (states[i][j] == OK || states[i][j] == HYP || states[i][j] == H_OK) {
            if (nb == 0 && !flagFromLast) {
                flagBetweenAll = false;
            }
            flagFromLast = false;
            nb++;
            if (i == width - 1) {
                line[++total] = nb;
                if (nbmax < nb) nbmax=nb;
                nb = 0;
            } else if (states[i+1][j] != OK && states[i+1][j] != HYP 
            && states[i+1][j] != H_OK) {
                line[++total] = nb;
                if (nbmax < nb) nbmax=nb;
                nb = 0;
            }
        }
        if (states[i][j] == KO || states[i][j] == H_KO) {
            flagFromLast = true;
        }
    }
    if (total > 0) {
        line[0]=total;
    } else {
        total = 1;
        line[0] = 1;
        line[1] = 0;
    }
    if (total == nbValue) {
        bool isOk = true;
        for (int i = 0; i < nbValue; i++) {
            if (line[i+1] != lines[j][i+1]) {
                isOk = false;
                break;
            }
        }
        if (isOk) {
            for (int i = 0; i < nbValue; i++) {
                lineColor[j][i] = 1;
            }
            return;
        }
    } else if (total > nbValue && flagBetweenAll) { // error -> too many values
        for (int k = 0; k < nbValue; k++) {
            lineColor[j][k] = 2;
        }
        return;
    }
    // check max values
    int max = 0;
    for (int i = 0; i < nbValue; i++) {
        int value = lines[j][i+1];
        if (max < value) max=value;
    }
    if (nbmax > max) {// error
        for (int k = 0; k < nbValue; k++) {
            lineColor[j][k] = 2;
        }
        return;
    }
    
    
    // check by the left
    total = 0;
    nb = 0;
    for (int i = 0; i < width; i++) {
        switch (states[i][j]) {
            case EMPTY : i = width; break;
            case OK :
            case HYP :
            case H_OK :
                nb++;
                if (nb == 1 && total == nbValue) {
                    for (int k = 0; k < nbValue; k++) {
                        lineColor[j][k] = 2;
                    }
                    return;
                }
                if (i + 1 < width) {
                    break;
                }
            case KO :
            case H_KO :
                if (nb > 0) {
                    if (total < nbValue) {
                        if (nb == lines[j][total+1]) {
                            lineColor[j][total] = 1;
                            nb = 0;
                            total++;
                        } else { // error
                            for (int k = 0; k < nbValue; k++) {
                                lineColor[j][k] = 2;
                            }
                            return;
                        }
                    } else { // error
                        for (int k = 0; k < nbValue; k++) {
                            lineColor[j][k] = 2;
                        }
                        return;
                    }
                }
                if (i + 1 == width && total < nbValue) { // error
                    for (int k = 0; k < nbValue; k++) {
                        lineColor[j][k] = 2;
                    }
                    return;
                }
                break;
        }
    }
    
    // check by the right
    total = 0;
    nb = 0;
    for (int i = width - 1; i >= 0; i--) {
        switch (states[i][j]) {
            case EMPTY : i = -1; break;
            case OK :
            case HYP :
            case H_OK :
                nb++;
                if (nb == 1 && total == nbValue) {
                    for (int k = 0; k < nbValue; k++) {
                        lineColor[j][k] = 2;
                    }
                    return;
                }
                if (i > 0) {
                    break;
                }
            case KO :
            case H_KO :
                if (nb > 0) {
                    if (total < nbValue) {
                        if (nb == lines[j][nbValue - total]) {
                            lineColor[j][nbValue - total - 1] = 1;
                            nb = 0;
                            total++;
                        } else { // error
                            for (int k = 0; k < nbValue; k++) {
                                lineColor[j][k] = 2;
                            }
                            return;
                        }
                    } else { // error
                        for (int k = 0; k < nbValue; k++) {
                            lineColor[j][k] = 2;
                        }
                        return;
                    }
                }
                // shouldn't happen
                if (i == 0 && total < nbValue) { // error
                    for (int k = 0; k < nbValue; k++) {
                        lineColor[j][k] = 2;
                    }
                    return;
                }
                break;
        }
    }
}

void Picross::fillColumn(int i) {
    int total = 0;
    int nb = 0;
    
    for (int j = 0; j < height; j++) {
        if (hidden[i][j] == OK) {
            nb++;
            if (j == height - 1) {
                columns[i][++total] = nb;
                nb = 0;
            } else if (hidden[i][j+1] != OK) {
                columns[i][++total] = nb;
                nb = 0;
            }
        }
    }
    
    if (total > 0) {
        columns[i][0]=total;
    } else {
        columns[i][0]=1;
        columns[i][1]=0;
    }
}

void Picross::fillLine(int j) {
    int total = 0;
    int nb = 0;
    
    for (int i = 0; i < width; i++) {
        if (hidden[i][j] == OK) {
            nb++;
            if (i == width - 1) {
                lines[j][++total] = nb;
                nb = 0;
            } else if (hidden[i+1][j] != OK) {
                lines[j][++total] = nb;
                nb = 0;
            }
        }
    }
    
    if (total > 0) {
        lines[j][0]=total;
    } else {
        lines[j][0]=1;
        lines[j][1]=0;
    }
}


void Picross::updateValuesForCreation() {
    int startX;
    int startY;
    int stopX;
    int stopY;
    if (cursorOldX > -1 && cursorOldY > -1) {
        startX = (cursorX < cursorOldX ? cursorX : cursorOldX);
        startY = (cursorY < cursorOldY ? cursorY : cursorOldY);
        stopX = (cursorX > cursorOldX ? cursorX : cursorOldX);
        stopY = (cursorY > cursorOldY ? cursorY : cursorOldY);
    } else {
        startX = stopX = cursorX;
        startY = stopY = cursorY;
    }
    for (int i = startX ; i <= stopX; i++) {
        updateColumnForCreation(i);
    }
    for (int j = startY ; j <= stopY; j++) {
        updateLineForCreation(j);
    }
    
}

void Picross::updateColumnForCreation(int i) {
    int total = 0;
    int nb = 0;
    
    for (int j = 0; j < height; j++) {
        if (states[i][j] == OK) {
            nb++;
            if (j == height - 1) {
                columns[i][++total] = nb;
                nb = 0;
            } else if (states[i][j+1] != OK) {
                columns[i][++total] = nb;
                nb = 0;
            }
        }
    }
    
    if (total > 0) {
        columns[i][0]=total;
    } else {
        columns[i][0]=1;
        columns[i][1]=0;
    }
}

void Picross::updateLineForCreation(int j) {
    int total = 0;
    int nb = 0;
    
    for (int i = 0; i < width; i++) {
        if (states[i][j] == OK) {
            nb++;
            if (i == width - 1) {
                lines[j][++total] = nb;
                nb = 0;
            } else if (states[i+1][j] != OK) {
                lines[j][++total] = nb;
                nb = 0;
            }
        }
    }
    
    if (total > 0) {
        lines[j][0]=total;
    } else {
        lines[j][0]=1;
        lines[j][1]=0;
    }
}

void Picross::handleEvent(Event* event) {
    event->ERROR = false;
    
    if (!ready || victory || defeat || pause) {
        return;
    }
    
    // synchro with mouse events
    if (useMouse) {
        handleMouseEvent(event);
    }
    
    // if victory == false
    if (event->HYPOTHESE) {
        if (!hypothese) {
            if (states[cursorX][cursorY] == EMPTY) {
                states[cursorX][cursorY] = HYP;
                hypothese = true;
            }
        } else {
            for (int i = 0; i < MAX_COLUMNS; i++) {
                for (int j = 0; j < MAX_LINES; j++) {
                    if (states[i][j] == HYP || states[i][j] == H_OK 
                    || states[i][j] == H_KO) {
                        states[i][j] = EMPTY;
                    }
                }
            }
            hypothese = false;
        }
        for (int i = 0 ; i < width; i++) {
            fillColumn(i);
            updateColumnColor(i); // if there is a 0
        }
        for (int j = 0 ; j < height; j++) {
            fillLine(j);
            updateLineColor(j); // if there is a 0
        }
        return;
    }
    if (event->UP) {
        cursorY--;
        if (cursorY < 0) cursorY = height - 1;
    }
    if (event->DOWN) {
        cursorY++;
        if (cursorY > height - 1) cursorY = 0;
    }
    if (event->LEFT) {
        cursorX--;
        if (cursorX < 0) cursorX = width - 1;
    }
    if (event->RIGHT) {
        cursorX++;
        if (cursorX > width - 1) cursorX = 0;
    }
    if (event->ACTION || event->FLAG) {
        int stateX;
        int stateY;
        int w;
        int h;
        if (cursorStartX > -1 && cursorStartY > -1) {
            stateX = (cursorX < cursorStartX ? cursorX : cursorStartX);
            stateY = (cursorY < cursorStartY ? cursorY : cursorStartY);
            w = ((cursorX < cursorStartX ? 
                cursorStartX - cursorX : cursorX - cursorStartX) + 1);
            h = ((cursorY < cursorStartY ? 
                cursorStartY - cursorY : cursorY - cursorStartY) + 1);
        } else {
            stateX = cursorX;
            stateY = cursorY;
            w = 1;
            h = 1;
        }
        CellState toFill = states[cursorStartX][cursorStartY];
        bool activ = false;
        for (int j = stateY; j < stateY + h; j++) {
            for (int i = stateX; i < stateX + w; i++) {
                CellState& state = states[i][j];
                if (event->ACTION) {
                    if (state == EMPTY && toFill == EMPTY) {
                        if (hidden[i][j] == KO && (mode == NORMAL && !hypothese)) {
                            event->ERROR = true;
                            state = KO;
                            break;
                        } else {
                            activ = true;
                            if (hypothese) {
                                state = H_OK;
                            } else {
                                state = OK;
                            }
                        }
                    }
                    else if (((state == KO && toFill == KO) 
                    || (state == OK && toFill == OK && mode == LIBRE)) 
                    && !hypothese) {
                        state = EMPTY;
                    }
                    else if (((state == H_KO && toFill == H_KO) 
                    || (state == H_OK && toFill == H_OK)) && hypothese) {
                        state = EMPTY;
                    }
                } else if (event->FLAG) {
                    if (state == EMPTY && toFill == EMPTY) {
                        if (hypothese) {
                            state = H_KO;
                        } else {
                            activ = true;
                            state = KO;
                        }
                    }
                    else if (((state == KO && toFill == KO) 
                    || (state == OK && toFill == OK && mode == LIBRE)) 
                    && !hypothese) {
                        state = EMPTY;
                    }
                    else if (((state == H_KO && toFill == H_KO) 
                    || (state == H_OK && toFill == H_OK)) && hypothese) {
                        state = EMPTY;
                    }
                }
            }
            if (event->ERROR) break;
        }
        if (event->ACTION) {
            if (activ && !event->ERROR) {
                if (w == 1 && h == 1) {
                    Audio::getInstance()->playSound(11);
                } else {
                    Audio::getInstance()->playSound(12);
                }
            }
        } else if (event->FLAG) {
            if (activ) {
                if (w == 1 && h == 1) {
                    Audio::getInstance()->playSound(13);
                } else {
                    Audio::getInstance()->playSound(7);
                }
            }
        }
        
        cursorOldX = cursorStartX;
        cursorOldY = cursorStartY;
        cursorStartX = -1;
        cursorStartY = -1;
        
        updateColor();
        if ((event->ACTION || mode == LIBRE) && !hypothese) {
            checkVictory();
            if (victory) {
                theme = Resources::getInstance()->getTheme(1);
                color1 = getPixel(theme, 21, 0);
                color2 = getPixel(theme, 22, 0);
                color6 = getPixel(theme, 26, 0);
                color7 = getPixel(theme, 27, 0);
                color8 = getPixel(theme, 28, 0);
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < height; j++) {
                        if (states[i][j] == KO) {
                            states[i][j] = EMPTY;
                        }
                    }
                }
            }
        }
        
    }
    if ((event->HOLD_ACTION || event->HOLD_FLAG) && cursorStartX==-1 && cursorStartY==-1) {
        cursorStartX = cursorX;
        cursorStartY = cursorY;
    }
    if (event->CANCEL_ACTION || event->CANCEL_FLAG) {
        cursorStartX = -1;
        cursorStartY = -1;
    }
    
    if (event->MOUSE_ON_OFF) {
        useMouse = !useMouse;
        if (useMouse) {
            xmouse = PICROSS_X + cursorX * 8 + 4;
            ymouse = PICROSS_Y + cursorY * 8 + 4;
//            SDL_WarpMouse(xmouse, ymouse);
        }
    }
    
    
    if (event->MUSIC_ON_OFF) {
        mute = !mute;
        if (mute) {
            Audio::getInstance()->stopMusic();
        } else {
            Audio::getInstance()->replayMusic();
        }
    }
}

void Picross::handleMouseEvent(Event* event) {
//    SDL_GetMouseState(&xmouse, &ymouse);
    
    event->UP = false;
    event->DOWN = false;
    event->LEFT = false;
    event->RIGHT = false;
    
    event->ACTION = event->MOUSE_ACTION;
    event->HOLD_ACTION = event->MOUSE_HOLD_ACTION;
    event->FLAG = event->MOUSE_FLAG;
    event->HOLD_FLAG = event->MOUSE_HOLD_FLAG;
    event->CANCEL_ACTION = event->MOUSE_CANCEL_ACTION;
    event->CANCEL_FLAG = event->MOUSE_CANCEL_FLAG;
    
    bool toReset = false;
    if (xmouse < xmin) {xmouse = xmin; toReset = true;}
    if (xmouse > xmax) {xmouse = xmax; toReset = true;}
    if (ymouse < ymin) {ymouse = ymin; toReset = true;}
    if (ymouse > ymax) {ymouse = ymax; toReset = true;}
//    if (toReset) SDL_WarpMouse(xmouse, ymouse);
    
    cursorX = (xmouse - xmin) / 8;
    cursorY = (ymouse - ymin) / 8;
}

void Picross::loop() {
    if (!ready) {
        return;
    }
}

void Picross::useKeyboard() {
    useMouse = false;
}

void Picross::draw(SDL_Surface* gpScreen) {
    if (!ready) {
        return;
    }
    SDL_Rect src;
    SDL_Rect dst;
    
    // lines for values
    for (int j = 0; j < height; j++) {
        src.x=0; src.w=x; src.y=y+j*8; src.h=8;
        Uint32 color = (j == cursorY) ? color8 : (j%2 == 0) ? color6 : color7;
        SDL_FillRect(gpScreen, &src, color);
    }
    // columns for values
    for (int i = 0; i < width; i++) {
        src.x=x+i*8; src.w=8; src.y=0; src.h=y;
        Uint32 color = (i == cursorX) ? color8 : (i%2 == 0) ? color6 : color7;
        SDL_FillRect(gpScreen, &src, color);
    }
     
    // picross
    int startX = x/*+(width%2 == 0 ? width/2 : (width/2)+1)*SIZE_BETWEEN_LINE_VALUES*/;
    int startY = y/*+(height%2 == 0 ? height/2 : (height/2)+1)*SIZE_BETWEEN_COLUMN_VALUES*/;
    int endX = startX+width*8;
    int endY = startY+height*8;
    
    src.w = 7; src.h = 7;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            switch(states[i][j]) {
                case EMPTY : src.x = 0; src.y = 0; break;
                case OK : src.x=7; src.y = 0; break;
                case KO : src.x=14; src.y = 0; break;
                case HYP : src.x = 0; src.y = 7; break;
                case H_OK : src.x=7; src.y = 7; break;
                case H_KO : src.x=14; src.y = 7; break;
                default : continue;
            }
            dst.x = startX + 1 + 8 * i;
            dst.y = startY + 1 + 8 * j;
            SDL_BlitSurface(theme, &src, gpScreen, &dst);
        }
    }
    //first lines
    for (int i = startX; i <= endX; i+=8) {
        src.x=i; src.w=1; src.y=startY; src.h=endY-startY+1;
        SDL_FillRect(gpScreen, &src, color1);
    }
    for (int j = startY; j <= endY; j+=8) {
        src.x=startX; src.w=endX-startX+1; src.y=j; src.h=1;
        SDL_FillRect(gpScreen, &src, color1);
    }
    
    //second lines
    for (int i = startX+5*8; i < endX; i+=(5*8)) {
        src.x=i; src.w=1; src.y=startY; src.h=endY-startY+1;
        SDL_FillRect(gpScreen, &src, color2);
    }
    for (int j = startY+5*8; j < endY; j+=(5*8)) {
        src.x=startX; src.w=endX-startX+1; src.y=j; src.h=1;
        SDL_FillRect(gpScreen, &src, color2);
    }
    
    if (victory || defeat || pause) {
        return;
    }
    
    // cursor
    int x1, w1, y1, h1;
    
    if (cursorStartX > -1 && cursorStartY > -1) {
        x1 = startX + (cursorX < cursorStartX ? cursorX : cursorStartX) * 8;
        y1 = startY + (cursorY < cursorStartY ? cursorY : cursorStartY) * 8;
        w1 = ((cursorX < cursorStartX ? 
                cursorStartX - cursorX : cursorX - cursorStartX) + 1) * 8 + 1;
        h1 = ((cursorY < cursorStartY ? 
                cursorStartY - cursorY : cursorY - cursorStartY) + 1) * 8 + 1;
        drawCursor(gpScreen, x1, y1, w1, h1, color3);
        
        x1 = startX + cursorStartX * 8;
        y1 = startY + cursorStartY * 8;
        w1 = 9;
        h1 = 9;
        drawCursor(gpScreen, x1, y1, w1, h1, color4);
        
        x1 = startX + cursorX * 8;
        y1 = startY + cursorY * 8;
        w1 = 9;
        h1 = 9;
        drawCursor(gpScreen, x1, y1, w1, h1, color5);
    }
    else {
        x1 = startX + cursorX * 8;
        y1 = startY + cursorY * 8;
        w1 = 9;
        h1 = 9;
        drawCursor(gpScreen, x1, y1, w1, h1, color3);
    }
    
    
    
    //values
    //lines
    for (int i = 0; i < height; i++) {
        int xValue = startX;
        for (int j = lines[i][0]; j > 0; j--) {
            xValue-=SIZE_BETWEEN_LINE_VALUES;
            if (lines[i][j] > 9) xValue-=4;
            drawValue(gpScreen, lines[i][j], xValue, startY + 2 + 8 * i, lineColor[i][j-1]);
            if (lines[i][j] > 9) xValue-=1;
        }
    }
    //columns
    for (int i = 0; i < width; i++) {
        int yValue = startY;
        for (int j = columns[i][0]; j > 0; j--) {
            yValue-=SIZE_BETWEEN_COLUMN_VALUES;
            int tmp = startX + 2 + 8 * i;
            if (columns[i][j] > 9) tmp-=2;
            drawValue(gpScreen, columns[i][j], tmp, yValue, columnColor[i][j-1]);
        }
    }
    
    if (useMouse) {
        drawMouse(gpScreen);
    }
}

void Picross::drawValue(SDL_Surface* gpScreen, int value, int x1, int y1, int color) {
    int currentValue = value;
    if (value > 9) {
        currentValue = value/10;
        //if (currentValue == 1) x1++;
        drawChiffre(gpScreen, currentValue, x1, y1, color);
        //if (currentValue == 1) x1-=1;
        x1+=4;
        currentValue = value%10;
        if (currentValue == 1) x1-=1;
    }
    drawChiffre(gpScreen, currentValue, x1, y1, color);
}

void Picross::drawChiffre(SDL_Surface* gpScreen, int value, int x1, int y1, int color) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = value * 3; src.y = color * 5; src.w = 3; src.h = 5;
    dst.x = x1; dst.y = y1;
    SDL_BlitSurface(chiffres, &src, gpScreen, &dst);
}

void Picross::drawCursor(SDL_Surface* gpScreen, int x1, int y1, int w1, int h1, 
Uint32 color) {
    SDL_Rect src;
    
    src.x=x1; src.y=y1; src.w=w1; src.h=1;
    SDL_FillRect(gpScreen, &src, color);
    src.y+=h1-1;
    SDL_FillRect(gpScreen, &src, color);
    src.y-=(h1-1); src.h=h1; src.w=1;
    SDL_FillRect(gpScreen, &src, color);
    src.x+=(w1-1);
    SDL_FillRect(gpScreen, &src, color);
    
}

void Picross::drawMouse(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    src.x = 0; src.y = 0; src.w = 8; src.h = 8;
    dst.x = xmouse; dst.y = ymouse;
    SDL_BlitSurface(mouse, &src, gpScreen, &dst);
}
