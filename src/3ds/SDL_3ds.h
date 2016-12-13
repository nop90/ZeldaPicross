
#ifndef __OS_3DS_H__
#define __OS_3DS_H__


#include <stdlib.h>
#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

#define TICKS_PER_MSEC (268123.480)

#define SDL_INIT_VIDEO 1
#define SDL_INIT_AUDIO 2

typedef unsigned int Uint32;
typedef u16 Uint16;
typedef u8 Uint8;

// events

/** Keysym structure
 *
 *  - The scancode is hardware dependent, and should not be used by general
 *    applications.  If no hardware scancode is available, it will be 0.
 *
 *  - The 'unicode' translated character is only available when character
 *    translation is enabled by the SDL_EnableUNICODE() API.  If non-zero,
 *    this is a UNICODE character corresponding to the keypress.  If the
 *    high 9 bits of the character are 0, then this maps to the equivalent
 *    ASCII character:
 *      @code
 *	char ch;
 *	if ( (keysym.unicode & 0xFF80) == 0 ) {
 *		ch = keysym.unicode & 0x7F;
 *	} else {
 *		An international character..
 *	}
 *      @endcode
 */
typedef struct SDL_keysym {
	u8 scancode;			/**< hardware specific scancode */
	u32 sym;//	SDLKey sym;			/**< SDL virtual keysym */
	u32 mod;//	SDLMod mod;			/**< current key modifiers */
	u16 unicode;			/**< translated character */
} SDL_keysym;


/** @name General keyboard/mouse state definitions */
/*@{*/
#define SDL_RELEASED	0
#define SDL_PRESSED	1
/*@}*/

/** Event enumerations */
typedef enum {
       SDL_NOEVENT = 0,			/**< Unused (do not remove) */
       SDL_ACTIVEEVENT,			/**< Application loses/gains visibility */
       SDL_KEYDOWN,			/**< Keys pressed */
       SDL_KEYUP,			/**< Keys released */
       SDL_MOUSEMOTION,			/**< Mouse moved */
       SDL_MOUSEBUTTONDOWN,		/**< Mouse button pressed */
       SDL_MOUSEBUTTONUP,		/**< Mouse button released */
       SDL_JOYAXISMOTION,		/**< Joystick axis motion */
       SDL_JOYBALLMOTION,		/**< Joystick trackball motion */
       SDL_JOYHATMOTION,		/**< Joystick hat position change */
       SDL_JOYBUTTONDOWN,		/**< Joystick button pressed */
       SDL_JOYBUTTONUP,			/**< Joystick button released */
       SDL_QUIT,			/**< User-requested quit */
       SDL_SYSWMEVENT,			/**< System specific event */
       SDL_EVENT_RESERVEDA,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVEDB,		/**< Reserved for future use.. */
       SDL_VIDEORESIZE,			/**< User resized video mode */
       SDL_VIDEOEXPOSE,			/**< Screen needs to be redrawn */
       SDL_EVENT_RESERVED2,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVED3,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVED4,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVED5,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVED6,		/**< Reserved for future use.. */
       SDL_EVENT_RESERVED7,		/**< Reserved for future use.. */
       /** Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
       SDL_USEREVENT = 24,
       /** This last event is only for bounding internal arrays
	*  It is the number of bits in the event mask datatype -- Uint32
        */
       SDL_NUMEVENTS = 32
} SDL_EventType;

/** @name Predefined event masks */
/*@{*/
#define SDL_EVENTMASK(X)	(1<<(X))
typedef enum {
	SDL_ACTIVEEVENTMASK	= SDL_EVENTMASK(SDL_ACTIVEEVENT),
	SDL_KEYDOWNMASK		= SDL_EVENTMASK(SDL_KEYDOWN),
	SDL_KEYUPMASK		= SDL_EVENTMASK(SDL_KEYUP),
	SDL_KEYEVENTMASK	= SDL_EVENTMASK(SDL_KEYDOWN)|
	                          SDL_EVENTMASK(SDL_KEYUP),
	SDL_MOUSEMOTIONMASK	= SDL_EVENTMASK(SDL_MOUSEMOTION),
	SDL_MOUSEBUTTONDOWNMASK	= SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN),
	SDL_MOUSEBUTTONUPMASK	= SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
	SDL_MOUSEEVENTMASK	= SDL_EVENTMASK(SDL_MOUSEMOTION)|
	                          SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)|
	                          SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
	SDL_JOYAXISMOTIONMASK	= SDL_EVENTMASK(SDL_JOYAXISMOTION),
	SDL_JOYBALLMOTIONMASK	= SDL_EVENTMASK(SDL_JOYBALLMOTION),
	SDL_JOYHATMOTIONMASK	= SDL_EVENTMASK(SDL_JOYHATMOTION),
	SDL_JOYBUTTONDOWNMASK	= SDL_EVENTMASK(SDL_JOYBUTTONDOWN),
	SDL_JOYBUTTONUPMASK	= SDL_EVENTMASK(SDL_JOYBUTTONUP),
	SDL_JOYEVENTMASK	= SDL_EVENTMASK(SDL_JOYAXISMOTION)|
	                          SDL_EVENTMASK(SDL_JOYBALLMOTION)|
	                          SDL_EVENTMASK(SDL_JOYHATMOTION)|
	                          SDL_EVENTMASK(SDL_JOYBUTTONDOWN)|
	                          SDL_EVENTMASK(SDL_JOYBUTTONUP),
	SDL_VIDEORESIZEMASK	= SDL_EVENTMASK(SDL_VIDEORESIZE),
	SDL_VIDEOEXPOSEMASK	= SDL_EVENTMASK(SDL_VIDEOEXPOSE),
	SDL_QUITMASK		= SDL_EVENTMASK(SDL_QUIT),
	SDL_SYSWMEVENTMASK	= SDL_EVENTMASK(SDL_SYSWMEVENT)
} SDL_EventMask ;
#define SDL_ALLEVENTS		0xFFFFFFFF
/*@}*/

/** Application visibility event structure */
typedef struct SDL_ActiveEvent {
	u8 type;	/**< SDL_ACTIVEEVENT */
	u8 gain;	/**< Whether given states were gained or lost (1/0) */
	u8 state;	/**< A mask of the focus states */
} SDL_ActiveEvent;

/** Keyboard event structure */
typedef struct SDL_KeyboardEvent {
	u8 type;	/**< SDL_KEYDOWN or SDL_KEYUP */
	u8 which;	/**< The keyboard device index */
	u8 state;	/**< SDL_PRESSED or SDL_RELEASED */
	SDL_keysym keysym;
} SDL_KeyboardEvent;

/** Mouse motion event structure */
typedef struct SDL_MouseMotionEvent {
	u8 type;	/**< SDL_MOUSEMOTION */
	u8 which;	/**< The mouse device index */
	u8 state;	/**< The current button state */
	u16 x, y;	/**< The X/Y coordinates of the mouse */
	s16 xrel;	/**< The relative motion in the X direction */
	s16 yrel;	/**< The relative motion in the Y direction */
} SDL_MouseMotionEvent;

/** Mouse button event structure */
typedef struct SDL_MouseButtonEvent {
	u8 type;	/**< SDL_MOUSEBUTTONDOWN or SDL_MOUSEBUTTONUP */
	u8 which;	/**< The mouse device index */
	u8 button;	/**< The mouse button index */
	u8 state;	/**< SDL_PRESSED or SDL_RELEASED */
	u16 x, y;	/**< The X/Y coordinates of the mouse at press time */
} SDL_MouseButtonEvent;

/** Joystick axis motion event structure */
typedef struct SDL_JoyAxisEvent {
	u8 type;	/**< SDL_JOYAXISMOTION */
	u8 which;	/**< The joystick device index */
	u8 axis;	/**< The joystick axis index */
	s16 value;	/**< The axis value (range: -32768 to 32767) */
} SDL_JoyAxisEvent;

/** Joystick trackball motion event structure */
typedef struct SDL_JoyBallEvent {
	u8 type;	/**< SDL_JOYBALLMOTION */
	u8 which;	/**< The joystick device index */
	u8 ball;	/**< The joystick trackball index */
	s16 xrel;	/**< The relative motion in the X direction */
	s16 yrel;	/**< The relative motion in the Y direction */
} SDL_JoyBallEvent;

/** Joystick hat position change event structure */
typedef struct SDL_JoyHatEvent {
	u8 type;	/**< SDL_JOYHATMOTION */
	u8 which;	/**< The joystick device index */
	u8 hat;	/**< The joystick hat index */
	u8 value;	/**< The hat position value:
			 *   SDL_HAT_LEFTUP   SDL_HAT_UP       SDL_HAT_RIGHTUP
			 *   SDL_HAT_LEFT     SDL_HAT_CENTERED SDL_HAT_RIGHT
			 *   SDL_HAT_LEFTDOWN SDL_HAT_DOWN     SDL_HAT_RIGHTDOWN
			 *  Note that zero means the POV is centered.
			 */
} SDL_JoyHatEvent;

/** Joystick button event structure */
typedef struct SDL_JoyButtonEvent {
	u8 type;	/**< SDL_JOYBUTTONDOWN or SDL_JOYBUTTONUP */
	u8 which;	/**< The joystick device index */
	u8 button;	/**< The joystick button index */
	u8 state;	/**< SDL_PRESSED or SDL_RELEASED */
} SDL_JoyButtonEvent;

/** The "window resized" event
 *  When you get this event, you are responsible for setting a new video
 *  mode with the new width and height.
 */
typedef struct SDL_ResizeEvent {
	u8 type;	/**< SDL_VIDEORESIZE */
	int w;		/**< New width */
	int h;		/**< New height */
} SDL_ResizeEvent;

/** The "screen redraw" event */
typedef struct SDL_ExposeEvent {
	u8 type;	/**< SDL_VIDEOEXPOSE */
} SDL_ExposeEvent;

/** The "quit requested" event */
typedef struct SDL_QuitEvent {
	u8 type;	/**< SDL_QUIT */
} SDL_QuitEvent;

/** A user-defined event type */
typedef struct SDL_UserEvent {
	u8 type;	/**< SDL_USEREVENT through SDL_NUMEVENTS-1 */
	int code;	/**< User defined event code */
	void *data1;	/**< User defined data pointer */
	void *data2;	/**< User defined data pointer */
} SDL_UserEvent;

/** If you want to use this event, you should include SDL_syswm.h */
struct SDL_SysWMmsg;
typedef struct SDL_SysWMmsg SDL_SysWMmsg;
typedef struct SDL_SysWMEvent {
	u8 type;
	void *msg;
} SDL_SysWMEvent;

/** General event structure */
typedef union SDL_Event {
	u8 type;
	SDL_ActiveEvent active;
	SDL_KeyboardEvent key;
	SDL_MouseMotionEvent motion;
	SDL_MouseButtonEvent button;
	SDL_JoyAxisEvent jaxis;
	SDL_JoyBallEvent jball;
	SDL_JoyHatEvent jhat;
	SDL_JoyButtonEvent jbutton;
	SDL_ResizeEvent resize;
	SDL_ExposeEvent expose;
	SDL_QuitEvent quit;
	SDL_UserEvent user;
	SDL_SysWMEvent syswm;
} SDL_Event;


/* The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

int SDL_Init(int flag); //returns 0 on success, -1 on failure 
void SDL_Quit();

u32 SDL_GetTicks();
void SDL_Delay(int ms);

// stubbed win functions
void SDL_WM_SetCaption(const char* caption,void* p);
void SDL_WM_SetIcon(void* i, void* p);


#define SDL_INIT_JOYSTICK 4


int SDL_PollEvent(SDL_Event * event);
int getTouchX();
int getTouchY();

int SDL_GetKeyState(void* p);

#define SDL_DISABLE 0
#define SDL_ENABLE 1
void SDL_ShowCursor(int s);

int getLanguage(void);
void setLanguage(int languageID);

int SDL_NumJoysticks();
void * SDL_JoystickOpen(int j);
void SDL_JoystickEventState(int f);
int SDL_JoystickNumButtons(void * joystick);
int SDL_JoystickGetButton(void * joystick, int i);
void SDL_JoystickClose(void * joystick);
void SDL_PumpEvents();
const char *  SDL_GetError();


#define SDLK_RETURN	KEY_A
#define SDLK_w		KEY_A
#define SDLK_x		KEY_B
#define SDLK_h		0//KEY_B
#define SDLK_ENTER	0//KEY_A//KEY_START
#define SDLK_d 		0//KEY_SELECT
#define SDLK_LEFT	KEY_LEFT
#define SDLK_RIGHT	KEY_RIGHT
#define SDLK_UP		KEY_UP
#define SDLK_DOWN	KEY_DOWN

#define SDLK_ESCAPE	KEY_START
#define SDLK_F4 	KEY_SELECT
#define KMOD_ALT 	KEY_START

#define SDLK_BUTTON_LEFT	0
#define SDLK_BUTTON_RIGHT	0

#define SDL_BUTTON_LEFT		0
#define SDL_BUTTON_RIGHT	0

#define SDLK_SPACE	0
#define SDLK_TAB 	0

#define SDLK_F1 	0
#define SDLK_COLON	0
#define SDLK_COMMA	0

#define KMOD_CAPS 0
#define KMOD_SHIFT 0

#define SDLK_CAPSLOCK 0
#define SDLK_LSHIFT 0
#define SDLK_LCTRL	0
#define SDLK_l		0
#define SDLK_r		0
#define SDLK_s		0
#define SDLK_z		0
#define SDLK_c		0
#define SDLK_p 		0
#define SDLK_m 		0
#define SDLK_t 		0
#define SDLK_o 		0
#define SDLK_SEMICOLON 0
#define SDLK_GREATER 0
#define SDLK_LESS 0
#define SDLK_ASTERISK 0
#define SDLK_HASH 0

#define SDLK_NUMLOCK 0
#define SDLK_SCROLLOCK 0

#define KMOD_CTRL 0

#define SDLK_0 0
#define SDLK_1 0
#define SDLK_2 0
#define SDLK_3 0
#define SDLK_4 0
#define SDLK_5 0
#define SDLK_6 0
#define SDLK_7 0
#define SDLK_8 0
#define SDLK_9 0

#define SDLK_KP_ENTER 0
#define SDLK_KP0 0
#define SDLK_KP1 0
#define SDLK_KP2 0
#define SDLK_KP3 0
#define SDLK_KP4 0
#define SDLK_KP5 0
#define SDLK_KP6 0
#define SDLK_KP7 0
#define SDLK_KP8 0
#define SDLK_KP9 0

#define SDLK_a 'a'
// GFX

/** Everything in the pixel format structure is read-only */
typedef struct SDL_PixelFormat {
	void *palette;//SDL_Palette *palette;
	u8  BitsPerPixel;
	u8  BytesPerPixel;
	u8  Rloss;
	u8  Gloss;
	u8  Bloss;
	u8  Aloss;
	u8  Rshift;
	u8  Gshift;
	u8  Bshift;
	u8  Ashift;
	u32 Rmask;
	u32 Gmask;
	u32 Bmask;
	u32 Amask;

	/** RGB color key information */
	u32 colorkey;
	/** Alpha value information (per-surface alpha) */
	u8  alpha;
} SDL_PixelFormat;

typedef struct{
  int x, y;
  int w, h;
} SDL_Rect;

typedef struct SDL_Surface {
        u32 flags;                           /* Read-only */
        SDL_PixelFormat *format;                /* Read-only */
        int w, h;                               /* Read-only */
        u16 pitch;                           /* Read-only */
        void *pixels;                           /* Read-write */
		sf2d_texture* texture;
		int colorKey;
        SDL_Rect clip_rect;                     /* Read-only */
        int refcount;                           /* Read-mostly */
} SDL_Surface;

typedef struct{
  int x, y;
} SDL_Point;


#define SDL_SRCCOLORKEY 1
#define SDL_SRCALPHA 2

#define SDL_FULLSCREEN 1
#define SDL_DOUBLEBUF 2
#define SDL_HWSURFACE 4
#define SDL_SWSURFACE 8
#define SDL_TOPSCREEN 16

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, int flags);

SDL_Surface* IMG_Load(const char* f);
SDL_Surface* SDL_CreateRGBSurface(int type, int width, int height, int bpp, int a, int b, int c, int d);

void SDL_LockSurface(SDL_Surface* s);
void SDL_UnlockSurface(SDL_Surface* s);

void SDL_FreeSurface(SDL_Surface* s);

void SDL_SetColorKey(SDL_Surface* s, int flag, u32 color);

void SDL_SetAlpha(SDL_Surface* s, int flag, u32 alpha);


u32 SDL_MapRGB(void* format , u8 r, u8 g, u8 b);


void SDL_FillRect(SDL_Surface* s, SDL_Rect* rect, u32 color);
void filledEllipseRGBA(SDL_Surface* s, int x, int y, int rx, int ry, u8 r, u8 g, u8 b, u8 a);

void SDL_BlitSurface(SDL_Surface* s, SDL_Rect * src, SDL_Surface* d, SDL_Rect * dst);

void SDL_Flip(void * p);

u32 GetPixel32(SDL_Surface * s, int x, int y);

typedef struct SDL_RWops
{
	u8* data;
	double size;
} SDL_RWops;


SDL_RWops* SDL_RWFromMem(unsigned char * buffer, unsigned int filesize);
void SDL_FreeRW(SDL_RWops * rw);

SDL_Surface* SDL_LoadBMP_RW(SDL_RWops* buffer, unsigned int x);

// SOUND

#define FSOUND_FREE 1
#define FSOUND_ALL 128

#define NUMSFX (45)

#define AUDIO_S16 SOUND_FORMAT_16BIT
#define MIX_MAX_VOLUME 256

#define AUDIO_S16SYS 1



typedef struct FSOUND_SAMPLE
{
	u8* data;
	u32 size;
	u32 format;
	u32 freq;
	bool used;
	bool loop;
	u64 usLen;
	u64 usStart;
	
} FSOUND_SAMPLE;


typedef FSOUND_SAMPLE FMUSIC_MODULE;

typedef FSOUND_SAMPLE Mix_Chunk;
typedef FSOUND_SAMPLE Mix_Music;

typedef FSOUND_SAMPLE HMUSIC;


void soundInit();
void soundClose();

int FSOUND_Init(u32 freq, u32 bpd, u32 unkn);

int FSOUND_GetSFXMasterVolume(); //
int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s); 


void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u32 volume);
void FMUSIC_FreeSong(FMUSIC_MODULE* s);
void FSOUND_SetSFXMasterVolume(u32 volson);

void initSFX(FSOUND_SAMPLE* s);
void load_SFX(FSOUND_SAMPLE* s, const char* filename, u32 format);

void FSOUND_PlaySound(int chl,FSOUND_SAMPLE* s);
void FSOUND_StopSound(int ch);
void FMUSIC_StopSong(FMUSIC_MODULE* s);
void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag);
void FMUSIC_PlaySong(FMUSIC_MODULE* s);

FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c);
FMUSIC_MODULE* FMUSIC_LoadSong(const char * f); 

void FSOUND_Sample_Free(FSOUND_SAMPLE* s);

void FSOUND_Close();

#endif