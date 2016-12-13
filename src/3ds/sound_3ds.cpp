
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#include "SDL_3ds.h"
#include "stb_vorbis.h"

#define MUSIC_GAIN 0 

bool soundEnabled;

typedef struct chStatus {
	u64 playlen;	
	u64 playstart;		
} chStatus;

chStatus soundchannels[8];

FSOUND_SAMPLE SFX[NUMSFX];

u32 SFXMasterVolume = 256;
u32 MasterVolume = 256;
int frequency=0;
int channel=0;

const char* stdMixErr = "SDL MIX ERROR";

int getFreeChannel()
{
	int startchannel = channel; 
	do
	{
		channel = (channel+1)%7;
		if (svcGetSystemTick()> soundchannels[channel].playlen + soundchannels[channel].playstart) return channel;
	} while(channel != startchannel);
	return -1;
}

void soundInit()
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		SFX[i].used=false;
	}

	for(i=0;i<8;i++)
	{
		soundchannels[i].playlen=0;
		soundchannels[i].playstart=0;
	}

	if(csndInit()==0)soundEnabled=true;
	else soundEnabled=false;
}

void soundClose()
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(SFX[i].used)
		{
			if(SFX[i].data)
			{
				linearFree(SFX[i].data);
				SFX[i].data=NULL;
			}
			SFX[i].used=false;
		}
	}
	if(soundEnabled)csndExit();
}

FILE* openFile(const char* fn, const char* mode)
{
	if(!fn || !mode)return NULL;
	return fopen(fn, mode);
}

void* bufferizeFile(const char* filename, u32* size, bool binary, bool linear)
{
	FILE* file;
	
	if(!binary)file = openFile(filename, "r");
	else file = openFile(filename, "rb");
	
	if(!file) return NULL;
	
	u8* buffer;
	long lsize;
	fseek (file, 0 , SEEK_END);
	lsize = ftell (file);
	rewind (file);
	if(linear)buffer=(u8*)linearMemAlign(lsize, 0x80);
	else buffer=(u8*)malloc(lsize);
	if(size)*size=lsize;
	
	if(!buffer)
	{
		fclose(file);
		return NULL;
	}
		
	fread(buffer, 1, lsize, file);
	fclose(file);
	return buffer;
}

int FSOUND_Init(u32 freq, u32 bps, u32 unkn)
{

	frequency = freq;
	
	return soundEnabled;
}

void initSFX(FMUSIC_MODULE* s)
{
	if(!s)return;

	s->data=NULL;
	s->size=0;
	s->used=true;
	s->loop=false;
}

void loadSFX(FMUSIC_MODULE* s, const char* filename, u32 format)
{
	if(!s)return;

	initSFX(s);

	s->data=(u8*) bufferizeFile(filename, &s->size, true, true);
	s->format=format;
	s->freq=frequency;
}

int FSOUND_GetSFXMasterVolume()
{
	return SFXMasterVolume;
}

int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s)
{
	return MasterVolume;
}

void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u32 volume)
{
	MasterVolume = volume;
	CSND_SetVol(15, CSND_VOL((float)MasterVolume/MIX_MAX_VOLUME, 0), CSND_VOL((float)MasterVolume/MIX_MAX_VOLUME, 0));
	csndExecCmds(0);
}

void FSOUND_SetSFXMasterVolume(u32 volson)
{
	SFXMasterVolume = volson;
}

void FSOUND_PlaySound(int ch,FSOUND_SAMPLE* s)
{
	if(!s || !s->used || !s->data || !soundEnabled || SFXMasterVolume == 0)return;
	int freech = getFreeChannel();
	if (freech>=0)
	{
		soundchannels[freech].playlen= ((u64)s->size * 1000 * TICKS_PER_MSEC) / s->freq;
		soundchannels[freech].playstart=svcGetSystemTick();
//		csndPlaySound(freech+8, s->format, s->freq, 1.0, 0.0, (u32*)s->data, (u32*)s->data, s->size);
		csndPlaySound(freech+8, s->format, s->freq, (float)SFXMasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
	}
}

void FSOUND_StopSound(int ch)
{
	if (ch==FSOUND_ALL)
	{
		int i;
		for(i=8;1<15;i++)
		{
			CSND_SetPlayState(i, 0);//Stop music audio playback fo all channels.
			csndExecCmds(0);
		}
	} else if(ch>7 && ch<15) {
		CSND_SetPlayState(ch, 0);
		csndExecCmds(0);
	}
}


void FMUSIC_StopSong(FMUSIC_MODULE* s)
{
	CSND_SetPlayState(15, 0);//Stop music audio playback.
	csndExecCmds(0);
}

void FMUSIC_PlaySong(FMUSIC_MODULE* s)
{
	int flag;
	if(!s || !s->used || !s->data || !soundEnabled) return;
	flag = s->format;
	if(s->loop) flag |= SOUND_REPEAT;
	soundchannels[7].playlen= ((u64)s->size * 1000 * TICKS_PER_MSEC) / s->freq;
	soundchannels[7].playstart=svcGetSystemTick();
//	csndPlaySound(15, flag,s->freq, 1.0, 0.0, (u32*)s->data, (u32*)s->data, s->size);
	csndPlaySound(15, flag,s->freq, (float)MasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
}


FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c)
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
printf("Load %s...",f);
			int channels;
			s16 * buffer;
			int freq;
			u32 scount;
			int len = stb_vorbis_decode_filename(f, &channels, &freq, &buffer);
			if (len<=0) {
printf("ko\n");
				if(buffer) free(buffer);
				return NULL;
			}
			if(channels==2) {
				SFX[i].data = (u8*) linearMemAlign(len/2, 0x80);
				if(!SFX[i].data) {
					if(buffer) free(buffer);
printf("ko\n");
					return NULL;
				}
				for (scount=0;scount<len/2;scount++) 
					SFX[i].data[scount]=buffer[scount*4]/256;
			} else if (channels==1) {
				SFX[i].data = (u8*) linearMemAlign(len/2, 0x80);
				if(!SFX[i].data) {
					if(buffer) free(buffer);
printf("ko\n");
					return NULL;
				}
				for (scount=0;scount<len/2;scount++) 
					SFX[i].data[scount]=buffer[scount*2]/256;
			} else {
				if(buffer) free(buffer);
printf("ko\n");
				return NULL;
			}
#if 0 // write on disk the decoded stream as raw pcm signed 8bit samples for debugging 
	char dumpbuf[255];
	sprintf(dumpbuf, "dump%i.raw", dumpcount++);
	if( buffer )
	{
		FILE* pf = fopen( dumpbuf, "wb" );
		if( pf )
		{
			fwrite( (u8*)SFX[i].data, 1, len, pf );
			fclose( pf );
		}
	}
#endif
			free(buffer);
			SFX[i].size= len/2;
			SFX[i].freq = freq/2;
			SFX[i].format = SOUND_FORMAT_8BIT;
			SFX[i].used = true;
			SFX[i].loop=false;
printf("ok\n");
			return &SFX[i];
		}
	}
	return NULL;
}

FMUSIC_MODULE* FMUSIC_LoadSong(const char * f)
{
	int size;
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
			loadSFX(&SFX[i], f, SOUND_FORMAT_8BIT);
			
			if(!SFX[i].data) return NULL;
			SFX[i].used = true;
			SFX[i].loop=false;
			SFX[i].freq=8000;
			return &SFX[i];
		}
	}
	return NULL;
}

void FSOUND_Close(){
	soundClose();
}


void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag)
{
	if (s)
		s->loop=flag;
}

void FSOUND_Sample_Free(FSOUND_SAMPLE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}


void FMUSIC_FreeSong(FMUSIC_MODULE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}
