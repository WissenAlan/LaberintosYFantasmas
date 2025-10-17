#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdlib.h>
#include "mapa.h"
#include "../red/cliente.h"
typedef struct {
    int is_running;
    tJugador p;
    tMapa m;
    tFantasma *f;
    tCola colaMov;
    int cantFant, cantVidas, cantPremios, cantVidasExt;
} tGame;
int game_new(tGame *game);
void game_free(tGame *game);
void game_run(tGame *game);
void game_update(tGame *g);

void moverFantasmas(tGame *g);
int crearConexion(tGame* g);
void iniciarJuego(tGame* g);
void asignarConfig(char*,int *parametro);
void desencolarMovs(tCola *cola, char ** mat, tJugador *p);
#endif // GAME_H_INCLUDED
