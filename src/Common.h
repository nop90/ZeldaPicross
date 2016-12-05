#ifndef __COMMON_H__
#define __COMMON_H__

#define ANIM_REFRESH    120     // intervale entre les anims

#define MAX_LINES      20
#define MAX_COLUMNS    25
#define MAX_NUMBER_LINES      ((MAX_COLUMNS/2)+1)
#define MAX_NUMBER_COLUMNS    ((MAX_LINES/2)+1)
#define SIZE_BETWEEN_LINE_VALUES    8
#define SIZE_BETWEEN_COLUMN_VALUES  6

#define NB_PICROSS      200
#define NB_MAPS         15
#define NB_PAGES        40
#define MAX_BY_PAGE     5

#define NB_BACKGROUND   7
#define NB_THEME        7
#define NB_MUSIC        12

#define NB_RANK         26

#define PICROSS_X       110
#define PICROSS_Y       70

#define TEXTS_BEFORE_DEST_LVL   NB_MAPS
#define TEXTS_BEFORE_TUTO       (TEXTS_BEFORE_DEST_LVL + NB_MAPS)
#define TEXTS_BEFORE_REQUIRED   (TEXTS_BEFORE_TUTO + 8)
#define TEXTS_BEFORE_FOUND      (TEXTS_BEFORE_REQUIRED + 9)
#define TEXTS_BEFORE_COMMON     (TEXTS_BEFORE_FOUND + 16)
#define TEXTS_BEFORE_MENU       (TEXTS_BEFORE_COMMON + 11)
#define TEXTS_BEFORE_BOSS       (TEXTS_BEFORE_MENU + 25)
#define TEXTS_BEFORE_PICROSS    (TEXTS_BEFORE_BOSS + 8)
#define TEXTS_BEFORE_INTRO      (TEXTS_BEFORE_PICROSS + 200)
#define TEXTS_BEFORE_ENDING     (TEXTS_BEFORE_INTRO + 3)
#define TEXTS_BEFORE_FREE       (TEXTS_BEFORE_ENDING + 9)
#define MAX_TEXTS               (TEXTS_BEFORE_FREE + 7)


#define COORD_MAP_X     70
#define COORD_MAP_Y     50
#define MAX_CASE_X      9
#define MAX_CASE_Y      6
#define TOTAL_CASE_MAP_SIZE     20
#define MAIN_CASE_MAP_SIZE      16
#define PATH_CASE_MAP_SIZE      2
#define ICON_SIZE               16


#define TIME_BEFORE_RUNNING     400
#define TIME_WHILE_RUNNING      100


#include <sstream>
#include <iostream>
#include <fstream>

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "3ds/SDL_3ds.h"

using namespace std;

enum QuestMode {WORLD_MAP, RESOLVE_PICROSS};

enum FreeMode {FREE_MAIN, SELECT_PICROSS, FREE_PICROSS};

enum PicrossMode {NORMAL, LIBRE}; //erreurs indiquées ou non

enum MenuMode {SELECT_SAVE, SELECT_MODE, PROFIL, DELETE};

enum EndingMode {TEXTS, RANK};

enum Mode {LOGO, TITLE, MENU, OPTIONS, RECORDS, INTRO, QUEST, ENDING, FREE};

enum CellState {EMPTY, OK, KO, HYP, H_OK, H_KO};

enum Object {NOTHING, SWORD, SHIELD, BOOTS, BOOK, POWER_GLOVES, BOMBS, BOW, 
    HOOKSHOT, FLIPPERS, HAMMER, MAP, COMPASS, KEYS, BOSS_KEY, 
    HEART, TRIFORCE, TOTAL_OBJ};

enum CaseMapState {TODO, DONE};

enum TransitionStep {IN, WAIT, OUT, END};

enum TransitionMode {NONE, CIRCLE};

enum Direction {UP, DOWN, LEFT, RIGHT};

#endif  // Common.h
