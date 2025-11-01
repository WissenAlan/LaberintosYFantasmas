#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#include "jugador.h"
#include "../estructuras/constantes.h"
#define ESPACIADO 2
#define VISITADO '.'
#define CAMINO '.'
#define CELDA ' '
#define BONIFICACION 'B'
typedef struct {
    int fil;
    int col;
} Celdas;
typedef struct{
    char **mat;
    int posyS;
    int posxS;
    int filMapa, colMapa;
    int exit;
    int jugadorMuerto;
} tMapa;
void llenarMat(char**mat, int fil, int col);
void mostrarMat(char**mat, int fil, int col);
char**crearMatriz(int filas, int columnas);
void eliminarMatriz(char**mat, int filas, int columnas);
int crearLaberinto(tMapa *m, int fil, int col, tJugador*p, int, int, int);
int buscarVecinos(char**mat, int fil, int col, Celdas*act, Celdas*vecinos);
void checkend(tMapa *m, tJugador *p);
int crearMapa(tMapa *m);
int llenarMapa(tJugador *p, tMapa* m , int, int, int);
void checklifes(tMapa *m, tJugador *pJug);
void ai(tCola *colamov,tMapa *mapa, tJugador *pJug, tFantasma *pFant);
int state(tMapa *mapa, int trypos,tJugador *pJug, const tFantasma *pFant);
#endif // FUNCIONES_H_INCLUDED
