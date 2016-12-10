/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <sstream>
#include <fstream>
#include <iostream>

//#include <SDL/SDL.h>
#include "3ds/SDL_3ds.h"
#include <3ds.h>
#include "Audio.h"


#include "Audio.h"

Audio Audio::instance=Audio();

Audio::Audio() : volume(0), musiqueId(0), playing(false) {
}
 

Audio::~Audio() {
    if (SOUND) {
        freeSounds();
        FMUSIC_StopSong(music);
        FMUSIC_SetMasterVolume(music, previous_volume);
        FMUSIC_FreeSong(music);
        FSOUND_SetSFXMasterVolume(previous_volson);
        FSOUND_Close();
    }
}

void Audio::init() {
   SOUND = FSOUND_Init(44100, 32, 0);
    music = NULL;
//SOUND=0; // to remove -> hack for no sound mode
//setVolume(32);
//setVolson(32);
    if (SOUND) {
        previous_volume = -1;
        previous_volson = FSOUND_GetSFXMasterVolume();
        loadSounds();
    }
}

Audio* Audio::getInstance() {
    return &instance;
}

void Audio::setVolume(int vol) {volume=vol*4;
    if (previous_volume == -1) previous_volume = FMUSIC_GetMasterVolume(music);
    FMUSIC_SetMasterVolume(music, volume);}
void Audio::setVolson(int volson) {FSOUND_SetSFXMasterVolume(volson*4);}

void Audio::loadSounds() {
    sons = new FSOUND_SAMPLE*[16];
    
    sons[0] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/text.ogg",0,0,0); // lettres
    sons[1] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu1.ogg",0,0,0); // menu 1
    sons[2] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu2.ogg",0,0,0); // menu 2
    sons[3] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu3.ogg",0,0,0); // menu 3
    sons[4] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/surprise.ogg",0,0,0); // surprise
    sons[5] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/monte.ogg",0,0,0); // monte
    sons[6] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/descend.ogg",0,0,0); // descend
    sons[7] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/bomb.ogg",0,0,0); // bombe
    sons[8] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/textnext.ogg",0,0,0); // suite texte
    sons[9] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/textend.ogg",0,0,0); // fin texte
    sons[10] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/happy.ogg",0,0,0); // trouve objet
    sons[11] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/sword.ogg",0,0,0); // épée
    sons[12] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/Sword360.ogg",0,0,0); // spin
    sons[13] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/stamp.ogg",0,0,0); // pose bombe
    sons[14] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/danger.ogg",0,0,0); // danger
    sons[15] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/hurt.ogg",0,0,0); // link se blesse
    
}

void Audio::freeSounds() {
    if (SOUND) {
        for (int i = 0; i < 16; i++) FSOUND_Sample_Free(sons[i]);
        delete[] sons;
    }
}

void Audio::playSound(int id, int chl) {
    if (SOUND) FSOUND_PlaySound(chl, sons[id]);
}

void Audio::stopSound() {
    if (SOUND) {FSOUND_StopSound(FSOUND_ALL); musiqueId=0;}
}

void Audio::playMusic(int id) {
    if (SOUND) {
        if (musiqueId != id || !playing) {
            musiqueId = id;
            FMUSIC_StopSong(music);
            FMUSIC_FreeSong(music);
            music = choixMusique(id);
            if (previous_volume == -1) previous_volume = FMUSIC_GetMasterVolume(music);
            FMUSIC_SetMasterVolume(music, volume);
            FMUSIC_SetLooping(music, 1);
            FMUSIC_PlaySong(music);
            playing = true;
        }
    }
}

void Audio::stopMusic() {
    if (SOUND) FMUSIC_StopSong(music);
    playing = false;
}

void Audio::replayMusic() {
    if (SOUND) FMUSIC_PlaySong(music);
    playing = true;
}

FMUSIC_MODULE* Audio::choixMusique(int id) {
return NULL; // nop90 - music disabled
    switch (id) {
        case 1 : return FMUSIC_LoadSong("romfs:/music/Plaine.ogg");
        case 2 : return FMUSIC_LoadSong("romfs:/music/Home.ogg");
        case 3 : return FMUSIC_LoadSong("romfs:/music/Lac.ogg");
        case 4 : return FMUSIC_LoadSong("romfs:/music/Cocorico.ogg");
        case 5 : return FMUSIC_LoadSong("romfs:/music/Mont.ogg");
        case 6 : return FMUSIC_LoadSong("romfs:/music/BoisPerdus.ogg");
        case 7 : return FMUSIC_LoadSong("romfs:/music/Jeu.ogg");
        case 8 : return FMUSIC_LoadSong("romfs:/music/Chateau.ogg");
        case 9 : return FMUSIC_LoadSong("romfs:/music/Boss.ogg");
        case 10 : return FMUSIC_LoadSong("romfs:/music/DFinal.ogg");
        case 11 : return FMUSIC_LoadSong("romfs:/music/BossF.ogg");
        case 12 : return FMUSIC_LoadSong("romfs:/music/Zelda.ogg");
        case 190 : return FMUSIC_LoadSong("romfs:/music/Selection.ogg");
        case 195 : return FMUSIC_LoadSong("romfs:/music/Magasin.ogg");
        case 200 : return FMUSIC_LoadSong("romfs:/music/Titre.ogg");
        case 210 : return FMUSIC_LoadSong("romfs:/music/Debut.ogg");
        case 220 : return FMUSIC_LoadSong("romfs:/music/Mort.ogg");
        case 230 : return FMUSIC_LoadSong("romfs:/music/Fin.ogg");
        default : return FMUSIC_LoadSong("romfs:/music/Plaine.ogg");
    }
}
