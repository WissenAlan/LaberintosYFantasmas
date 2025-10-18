#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdlib.h>
#include "jugador.h"
#include "mapa.h"
#include "../red/cliente.h"
typedef struct {
    int is_running;
    player *p;
    tMapa *m;
    ghost *f;
    tCola colaMov;
    int cantFant, cantVidas, cantPremios, cantVidasExt;
} tGame;
int game_new(tGame *game);
void game_free(tGame *game);
void game_run(tGame *game);
void game_update(tGame *g);

void moverfantasmas(tGame *g, tCola *cola);
int crearConexion(tGame* g);
int crearJugador(tGame* g);
void iniciarJuego(tGame* g);
void asignarConfig(char*,int *parametro);
#endif // GAME_H_INCLUDED
