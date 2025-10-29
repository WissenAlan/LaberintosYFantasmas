#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdlib.h>
#include "mapa.h"
#include "../red/cliente.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <stdbool.h>
typedef struct {
    tJugador p;
    tMapa m;
    tFantasma *f;
    tCola colaMov;
    int cantFant, cantVidas, cantPremios, cantVidasExt;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event eventos;
     bool is_running;
    bool is_pausing;
    bool inicio;
    bool ranking;
    bool is_writing;
    TTF_Font *text_f;
    TTF_Font *titulo_f;
    SDL_Texture *fondo;
    SDL_Texture *rank;
    SDL_Texture *personaje;
    SDL_Texture *fantasmas;
    Mix_Chunk *sonidomenu;
    Mix_Music *musica;
    Mix_Music *musicajuego;
} tGame;
int game_new(tGame *game);
void game_free(tGame *game);
void game_run(tGame *game);
void game_update(tGame *g);
void game_events(tGame *g);
void game_draw(tGame *g);
void moverFantasmas(tGame *g);
char* crearConexion(tGame* g, int);
void iniciarJuego(tGame* g);
void game_start(tGame *g);
void asignarConfig(char*,int *parametro);
void desencolarMovs(tCola *cola, char ** mat, tJugador *p);
#endif // GAME_H_INCLUDED
