#ifndef SDL_INIT_H_INCLUDED
#define SDL_INIT_H_INCLUDED
#include "game.h"
#define SDL_FLAGS SDL_INIT_EVERYTHING
#define IMG_FLAGS IMG_INIT_PNG
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#define WINDOW_TITLE "LabFan"
#define TITLEGAME "LabFan"
#define RENDERER_COLOR   0, 0, 0, 255
#define CUADRADO_COLOR   255, 255, 255, 255
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768
#define MIXER_FLAGS MIX_INIT_OGG
#define IMAGE_FLAGS IMG_INIT_PNG
int game_init_sdl(tGame *g);
void load_media(tGame *g);

#endif // SDL_INIT_H_INCLUDED
