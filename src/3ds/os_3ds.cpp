
#include <sf2d.h>
#include <sfil.h>
#include "SDL_3ds.h"

u8 language = 1;
const char* stdErr = "SDL ERROR";

void task_init();
void task_exit();


int SDL_Init(int flag) //returns 0 on success, -1 on failure 
{

	if(flag & SDL_INIT_VIDEO)
	{
		sf2d_init(); 
		sf2d_set_3D(false);
		sf2d_set_vblank_wait(true);
		
		task_init();
		
//		osSetSpeedupEnable(true);
		
		romfsInit();
		cfguInit();

#ifdef DEBUG
		consoleInit (GFX_BOTTOM, NULL);
#endif 

		// Read the language field from the config savegame.
		CFGU_GetSystemLanguage(&language);
		
    }
	if(flag & SDL_INIT_AUDIO)
		soundInit();

    return 0;
}


void SDL_Quit()
{
	soundClose();
	cfguExit();
    romfsExit();
	task_exit();
	sf2d_fini();

}

u32 SDL_GetTicks()
{
    return svcGetSystemTick()/TICKS_PER_MSEC;
}

void SDL_Delay(int ms)
{
	svcSleepThread(ms*TICKS_PER_MSEC);
}

// stubbed win functions
void SDL_WM_SetCaption(const char* caption,void* p) {return;}
void SDL_WM_SetIcon(void* i, void* p) {return;}
void SDL_ShowCursor(int s) {return;}

u8 eventstate=0;
u8 eventbit=0;
u8 muspaused=0;

int SDL_PollEvent(SDL_Event * event)
{

	if (eventstate==0)
	{
		hidScanInput();
		eventstate++;
		eventbit=0;

		if(!aptMainLoop())
		{
			event->type=SDL_QUIT;
			SDL_Quit();
			exit(0);
		}
		if (muspaused)
		{
			CSND_SetPlayState(15, 1);//Resume music audio playback.
			csndExecCmds(0);
			muspaused = 0;
		}
	}

	if (eventstate==1)
	{
		while(!(hidKeysDown() & (1<<eventbit)) && eventbit<16)
			eventbit++;
		if(eventbit>=16)	
		{
			eventstate++;
			eventbit=0;
		} else {
			event->type=SDL_KEYDOWN;
			event->key.keysym.sym = 1<<eventbit;
			eventbit++;
			return 1;
		}
	}
	if (eventstate==2)
	{
		while(!(hidKeysUp() & (1<<eventbit)) && eventbit<16)
			eventbit++;
		if(eventbit>=16)	
		{
			eventstate++;
			eventbit=28;
		} else {
			event->type=SDL_KEYUP;
			event->key.keysym.sym = 1<<eventbit;
			eventbit++;
			return 1;
		}
	}
	if (eventstate==3)
	{
		while(!(hidKeysDown() & (1<<eventbit)) && eventbit<32)
			eventbit++;
		if(eventbit>=32)	
		{
			eventstate++;
			eventbit=28;
		} else {
			event->type=SDL_JOYBUTTONDOWN;
			event->jbutton.button = 1<<eventbit;//(eventbit-28);
			eventbit++;
			return 1;
		}
	}
	if (eventstate==4)
	{
		while(!(hidKeysUp() & (1<<eventbit)) && eventbit<32)
			eventbit++;
		if(eventbit>=32)	
		{
			eventstate++;
			eventbit=0;
		} else {
			event->type=SDL_JOYBUTTONUP;
			event->jbutton.button = 1<<eventbit;//(eventbit-28);
			eventbit++;
			return 1;
		}
	}
	// add touchpad events
	eventstate=0;
	eventbit=0;
	return 0;
}

int getTouchX(){
	touchPosition pos;	
	hidTouchRead(&pos);
	return pos.px;
}

int getTouchY(){
	touchPosition pos;	
	hidTouchRead(&pos);
	return pos.py;
}

int SDL_GetKeyState(void* p)
{
	return hidKeysHeld();
}

int getLanguage(void)
{
	return language;
}

void setLanguage(int languageID)
{
	if (languageID>5) language = 1;
	else if (languageID<1) language = 5;
	else language = languageID;
 
}

int SDL_JoystickNumButtons(void * joystick){
	return 0;
}

int SDL_JoystickGetButton(void * joystick, int i){
	return 0;
}

void SDL_JoystickClose(void * joystick){
	return ;
}

int SDL_NumJoysticks(){
	return 0;
}

void * SDL_JoystickOpen(int j){
	return NULL;
}

void SDL_JoystickEventState(int f){}


void SDL_PumpEvents(){
	return;
}

const char * SDL_GetError(){
	return stdErr;
}

SDL_RWops* SDL_RWFromMem(unsigned char * buffer, unsigned int filesize)
{
    SDL_RWops* rw;
	rw = (SDL_RWops*) malloc(sizeof(SDL_RWops));
	if(!rw) return NULL;
	rw->data=buffer;
	rw->size=filesize;
	return rw;
}

void SDL_FreeRW(SDL_RWops * rw){
	if (rw) 
		free(rw);
}



static bool task_quit;
static Handle task_pause_event;
static Handle task_suspend_event;
static aptHookCookie cookie;

static void task_apt_hook(APT_HookType hook, void* param) {
    switch(hook) {
        case APTHOOK_ONSUSPEND:
            svcClearEvent(task_suspend_event);
			CSND_SetPlayState(15, 0);//Stop music audio playback.
			csndExecCmds(0);
			muspaused=1;
            break;
        case APTHOOK_ONSLEEP:
            svcClearEvent(task_pause_event);
            break;
        default:
            break;
    }
}

void task_init() {
    task_quit = false;

    svcCreateEvent(&task_pause_event, RESET_STICKY);

    svcCreateEvent(&task_suspend_event, RESET_STICKY);

    svcSignalEvent(task_pause_event);
    svcSignalEvent(task_suspend_event);

    aptHook(&cookie, task_apt_hook, NULL);
}

void task_exit() {
    task_quit = true;

    aptUnhook(&cookie);

    if(task_pause_event != 0) {
        svcCloseHandle(task_pause_event);
        task_pause_event = 0;
    }

    if(task_suspend_event != 0) {
        svcCloseHandle(task_suspend_event);
        task_suspend_event = 0;
    }
}

