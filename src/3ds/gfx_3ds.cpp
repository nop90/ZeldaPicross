
#include <stdio.h> // for debug with printf
#include "SDL_3ds.h"

bool drawing,drawing2;
int offset;
float scalepos;
float scalewidth;

void endTopFrame(void)
{
	if(drawing)
	{ 
		if (offset)
		{
			sf2d_draw_rectangle(0, 0, 40, 240, RGBA8(0, 0, 0, 0xff)); 
			sf2d_draw_rectangle(360, 0, 40, 240, RGBA8(0, 0, 0, 0xff));
		}
		sf2d_end_frame();
	}
	drawing = false;
}

void endBottomFrame(void)
{
	if(drawing2)
		sf2d_end_frame();
	drawing2 = false;
}

void SDL_Flip(void * p)
{
	endTopFrame();
	endBottomFrame();
	sf2d_swapbuffers();
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, int flag)
{
	drawing=false;
	drawing2=false;
	if(flag & SDL_FULLSCREEN)
	{
		offset = 0;
		scalepos = 1.25; 
		scalewidth = 1.26; //1.28; //should be 1.25 but setting a slight bigger value avoid tiles positioning defects caused by scaled width approximation
	} else {
		offset = 1;
		scalepos = 1; 
		scalewidth = 1; 
	}
    SDL_Surface* s;
	s = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	if(!s) return NULL;
	
	s->w = width;
	s->h = height;
	s->flags = 0;
	s->format = (SDL_PixelFormat*) malloc(sizeof(SDL_PixelFormat));

	if (flag & SDL_TOPSCREEN) 
		s->texture = (sf2d_texture*) 1;
	else 
		s->texture = (sf2d_texture*) 2;
	return s;
}

SDL_Surface* IMG_Load(const char* f)
{
    SDL_Surface* s;
	s = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	if(!s) return NULL;
	s->texture = sfil_load_PNG_file(f, SF2D_PLACE_RAM);
	if (s->texture) {
		s->w = s->texture->width;
		s->h = s->texture->height;
		sf2d_texture_set_params(s->texture, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR)); 
		s->flags = 0;
		s->format = 0;
		sf2d_texture_tile32(s->texture);
		return s;
	} else {
		free(s);
		return NULL;
	} 
}

SDL_Surface* SDL_CreateRGBSurface(int type, int width, int height, int bpp, int a, int b, int c, int d)
{
    SDL_Surface* s;
	s = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	if(!s) return NULL;
	s->texture = sf2d_create_texture (width, height,  TEXFMT_RGBA8, SF2D_PLACE_RAM);
	if (s->texture) {
		s->w = s->texture->width;
		s->h = s->texture->height;
		s->flags = 0;
		sf2d_texture_set_params(s->texture, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR)); 
		s->format = 0;
		sf2d_texture_tile32(s->texture);
		return s;
	} else {
		free(s);
		return NULL;
	} 
}


void SDL_FreeSurface(SDL_Surface* s)
{
    if(s)
	{
		if((s->texture==(sf2d_texture*) 1) || (s->texture==(sf2d_texture*) 2)) 
			s->texture=NULL;
		else
		{
			if(s->texture) sf2d_free_texture(s->texture);
		}
		if (s->format) free(s->format);
		free(s);
	}
}


void SDL_SetColorKey(SDL_Surface* s, int flag, u32 color)
{
	u32 alphamask = RGBA8(0xff, 0xff, 0xff,0);
	//Adjust to sf2d lib internal format
	u32 c =   ((color&0xff)<<24) + ((color&0xff00)<<8) + ((color&0xff0000)>>8) + ((color&0xff000000)>>24);
    s->colorKey= color;
	int x,y;
	for(x=0;x<s->texture->width;x++)
		for(y=0;y<s->texture->height;y++){
			if((sf2d_get_pixel(s->texture,x,y)&alphamask) ==  (c&alphamask)) sf2d_set_pixel(s->texture,x,y,0);
		}
}

void SDL_SetAlpha(SDL_Surface* s, int flag, u32 alpha){
	u32 alphaval = RGBA8(alpha & 0xff, 0, 0, 0); //sf2d lib internal format is ARGB
	u32 alphamask = RGBA8(0, 0xff, 0xff, 0xff);
	int x,y;
	for(x=0;x<s->texture->width;x++)
		for(y=0;y<s->texture->height;y++)
			sf2d_set_pixel(s->texture,x,y,(sf2d_get_pixel(s->texture,x,y)&alphamask)|alphaval);
}


u32 SDL_MapRGB(void * format , u8 r, u8 g, u8 b)
{
    return RGBA8(r, g, b, 0xff);
}

void SDL_FillRect(SDL_Surface* s, SDL_Rect* rect, u32 color)
{
    if (s->texture==(sf2d_texture*) 1) 
	{

		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 
		if(rect) sf2d_draw_rectangle(rect->x*scalepos+40*offset, rect->y, rect->w*scalewidth, rect->h, color);
		else  sf2d_draw_rectangle(40*offset, 0, 320 *scalepos, 240, color);
	} else if (s->texture==(sf2d_texture*) 2)
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 
		if(rect) sf2d_draw_rectangle(rect->x, rect->y, rect->w, rect->h, color);
		else  sf2d_draw_rectangle(0, 0, 320, 240, color);
	} else {
		if(s) 
			if(s->texture){ 
				int i,j;
				u32 c =   ((color&0xff000000)>>24) + (color<<8); // sf2d lib internal pixel format is ARGB

				int dx = (rect)?rect->x:0;
				int dy = (rect)?rect->y:0;
				int dw = (rect)?rect->w:s->texture->width;
				int dh = (rect)?rect->h:s->texture->height;
				if (dw > s->texture->width) dw = s->texture->width;
				if (dh > s->texture->height) dh = s->texture->height;
				for (i=dx;i< dx+dw; i++)
					for (j=dy;j< dy+dh; j++) 
						sf2d_set_pixel(s->texture,i,j,c);
				sf2d_texture_tile32(s->texture);
			}
	}
}

void filledEllipseRGBA(SDL_Surface* s, int x, int y, int rx, int ry, u8 r, u8 g, u8 b, u8 a)
{
    if (s->texture==(sf2d_texture*) 1) 
	{
		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 
		sf2d_draw_fill_circle(x*scalepos+40*offset, y, rx*scalewidth, RGBA8(r, g, b, a));
	} else if (s->texture==(sf2d_texture*) 2) 
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 
		sf2d_draw_fill_circle(x, y, rx, RGBA8(r, g, b, a));
	}
}

void SDL_BlitSurface(SDL_Surface* s, SDL_Rect * src, SDL_Surface* d, SDL_Rect * dst)
{
	u32 pixel;
    if(d->texture==(sf2d_texture*) 1)
	{
		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 

		if(src) {
			if(dst) 
				sf2d_draw_texture_part_scale(s->texture, (int)(dst->x*scalepos+40*offset),dst->y, src->x, src->y, src->w, src->h, scalewidth,1);
			else
				sf2d_draw_texture_part_scale(s->texture, 40*offset,0, src->x, src->y, src->w, src->h, scalewidth,1);
		} else {
			if(dst) 
				sf2d_draw_texture_scale(s->texture, (int)(dst->x*scalepos+40*offset),dst->y, scalewidth,1);
			else
				sf2d_draw_texture_scale(s->texture, 40*offset,0, scalewidth,1);
		}
	} else if(d->texture==(sf2d_texture*) 2) 
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 

		if(src) {
			if(dst) 
				sf2d_draw_texture_part(s->texture, dst->x,dst->y, src->x, src->y, src->w, src->h);
			else
				sf2d_draw_texture_part(s->texture, 0,0, src->x, src->y, src->w, src->h);
		} else {
			if(dst) 
				sf2d_draw_texture(s->texture, dst->x,dst->y);
			else
				sf2d_draw_texture(s->texture, 0,0);
		}
	} else {
		if(s && d) 
			if(s->texture && d->texture){ 
				int i,j;

				int sx = (src)?src->x:0;
				int sy = (src)?src->y:0;
				int dx = (dst)?dst->x:0;
				int dy = (dst)?dst->y:0;
				int sw = (src)?src->w:s->texture->width;
				int sh = (src)?src->h:s->texture->height;
				if (sx+sw > s->texture->width) sw = s->texture->width - sx;
				if (sy+sh > s->texture->height) sh = s->texture->height - sy;
				for (i=0;i< sw; i++)
					for (j=0;j< sh; j++) {
						pixel = sf2d_get_pixel ( s->texture,i+sx,j+sy);
						if (pixel & 0xff) sf2d_set_pixel(d->texture,i+dx,j+dy,pixel); 		
					}
				sf2d_texture_tile32(d->texture);
			}
	}
}


void SDL_LockSurface(SDL_Surface* s){}
void SDL_UnlockSurface(SDL_Surface* s){}

u32 GetPixel32(SDL_Surface * s, int x, int y)
{
	u32 temp = sf2d_get_pixel(s->texture,x,y);
	temp = (temp>>8) | 0xff000000; 
//	printf("%x\n",temp);
	return temp;
}



