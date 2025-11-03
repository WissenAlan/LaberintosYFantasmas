#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "../estructuras/Arbol.h"
#include <stdlib.h>
#include "mapa.h"
#include "../red/cliente.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "time.h"
typedef struct {
    tJugador p;
    tMapa m;
    tFantasma *f;
    tCola colaMov, colaMovsJugador;
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
    SDL_Texture *fondonombre;
    SDL_Texture *fantasmas;
    SDL_Texture *premio;
    SDL_Texture *entrada;
    SDL_Texture *salida;
    SDL_Texture *piso;
    SDL_Texture *pared;
    Mix_Chunk *sonidomenu;
    Mix_Music *musica;
    Mix_Music *musicajuego;
} tGame;

typedef struct {
    int idPartida;
    int idJugador;
    int puntaje;
    int movimientos;
} tPartida;
int game_new(tGame *game);
void game_free(tGame *game);
void game_run(tGame *game);
void game_update(tGame *g);
void game_events(tGame *g);
void game_draw(tGame *g);
void moverFantasmas(tGame *g);
int crearConexion(tGame *g);
void iniciarJuego(tGame* g);
void game_start(tGame *g);
void asignarConfig(char*,int *parametro);
void desencolarMovs(tCola *cola, char ** mat, tJugador *p);
void mostrarPartidas();
void guardarPartida(tGame *g);
void enviarMensaje(char* buffer, char* response);
void procesarGuardarPartida(tGame*g);
#endif // GAME_H_INCLUDED

