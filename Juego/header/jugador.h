#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include "../estructuras/colaDin.h"
#include <stdlib.h>
#include "../estructuras/constantes.h"
#include <conio.h>

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA -2
#define bufferNombreSize 7
#define bufferEmailSize 50
typedef struct {
    int posx;
    int posy;
    unsigned puntos;
    unsigned vidas;
    char nombre[bufferNombreSize];
    char email[bufferEmailSize];
    int roundBuff;
} tJugador;
typedef struct {
    int posx;
    int posy;
    int estado;
} tFantasma;
typedef struct {
    int posx;
    int posy;
    int movimiento;
    char entidad;
} tMovimiento;
void crearJugador(tJugador *pJug, int);
unsigned getVidasJugador(tJugador *pJug);
unsigned getPuntosJugador(tJugador *pJug);
void moverJugador(tCola *colamov, char **mat, tJugador *pJug, int);
void move_up(char**mat, tJugador *pJug);
void move_down(char**mat, tJugador *pJug);
void move_left(char**mat, tJugador *pJug);
void move_right(char**mat, tJugador *p);
int abs(int numero);
void CoordenadasesquinaMasLejosJugador(tJugador *jugador,int* filas,int* columnas);
int contarMovs(tCola* colaMovsJugador);
void encolarMov(tCola *cola, tMovimiento *movimiento, int mov);
#endif // JUGADOR_H_INCLUDED
